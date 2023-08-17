#include "mainwindow.h"
#include "ModificacoesQuadro/changeColor.h"
#include "ModificacoesQuadro/changeThickness.h"
#include "ModificacoesQuadro/clearCanvas.h"
#include "TransformDialog.h"
#include <QColorDialog>
#include <QInputDialog>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), currentColor(Qt::black), currentPenThickness(5) 
{
    resize(800, 600);
    this->setStyleSheet("background-color: #2E2E2E;");

    initializeUI();
}


void MainWindow::initializeUI() {
    canvas = QPixmap(600, 600);
    canvas.fill(Qt::white);
    
    //criar botao X 
    botaoX = new QLabel(this);
    botaoX->setGeometry(620, 10, 160, 20);

    //criar botao y
    botaoY = new QLabel(this);
    botaoY->setGeometry(620, 40, 160, 20);

    QFont font = botaoX->font();
    font.setPointSize(14);
    botaoX->setFont(font);
    botaoY->setFont(font);

    QString labelStyle = "border: 2px solid #FFFFFF; background-color: #444444; color: #FFFFFF; padding: 5px;";
    botaoX->setStyleSheet(labelStyle);
    botaoY->setStyleSheet(labelStyle);

    int labelWidth = 75;
    int labelHeight = 30;
    int spacing = 10;
    botaoX->setGeometry(620, 10, labelWidth, labelHeight);
    botaoY->setGeometry(620 + labelWidth + spacing, 10, labelWidth, labelHeight);

    int buttonWidth = 75;
    int buttonHeight = 30;
    int buttonSpacing = 10;

    QPushButton *colorPickerButton = new QPushButton("Cor", this);
    colorPickerButton->setGeometry(620, 400, buttonWidth, buttonHeight);
    connect(colorPickerButton, &QPushButton::clicked, this, &MainWindow::openColorPicker);

    QPushButton *adjustThicknessButton = new QPushButton("Tamanho", this);
    adjustThicknessButton->setGeometry(620 + buttonWidth + buttonSpacing, 400, buttonWidth, buttonHeight);
    connect(adjustThicknessButton, &QPushButton::clicked, this, &MainWindow::adjustPenThickness);

    QPushButton *clearButton = new QPushButton("Clear", this);
    clearButton->setGeometry(620, 400 + buttonHeight + buttonSpacing, buttonWidth, buttonHeight);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::clearCanvas);

    QPushButton *graphButton = new QPushButton("Gráfico", this);
    graphButton->setGeometry(620 + buttonWidth + buttonSpacing, 400 + buttonHeight + buttonSpacing, buttonWidth, buttonHeight);
    connect(graphButton, &QPushButton::clicked, this, &MainWindow::openTransformDialog);


    QPushButton *pointButton = new QPushButton("Pontos", this);
    pointButton->setGeometry(620 + 2*(buttonWidth + buttonSpacing), 400, buttonWidth, buttonHeight);
    connect(pointButton, &QPushButton::clicked, this, &MainWindow::setPointNumber);

    drawLineButton = new QPushButton("Desenhar Reta", this);
    drawLineButton->setGeometry(620 + 2*(buttonWidth + buttonSpacing), 400 + buttonHeight + buttonSpacing, buttonWidth, buttonHeight);
    connect(drawLineButton, &QPushButton::clicked, this, &MainWindow::drawLine);
    drawLineButton->hide();
}

void MainWindow::togglePointUI(bool show) {
    qDebug() << "Dentro do togglePointUI";

    if(!botaoX || !botaoY || !graphButton || !drawLineButton) {
        qDebug() << "Algum widget não foi inicializado corretamente.";
        return;
    }

    botaoX->setVisible(show);
    botaoY->setVisible(show);
    graphButton->setVisible(show);
    drawLineButton->setVisible(show);

    qDebug() << "Saindo do togglePointUI";
}

void MainWindow::setPointNumber() {
    qDebug() << "Inicio setPointNumber";
    bool ok;
    int numPoints = QInputDialog::getInt(this, "Número de Pontos", "Quantos pontos você quer criar?", 1, 1, 50, 1, &ok);
    if (ok) {
        qDebug() << "Usuario pressionou OK";
        numberOfPointsToCreate = numPoints;
        pointsCreated = 0;
        pointList.clear();

        // Remove labels antigos (caso existam)
        for (auto &label : pointXLabels) {
            delete label;
        }
        for (auto &label : pointYLabels) {
            delete label;
        }
        pointXLabels.clear();
        pointYLabels.clear();

        // Cria novos labels para os pontos
        for (int i = 0; i < numPoints; i++) {
            QLabel *xLabel = new QLabel(this);
            QLabel *yLabel = new QLabel(this);
            xLabel->hide();  // esconde até o ponto ser criado
            yLabel->hide();
            pointXLabels.push_back(xLabel);
            pointYLabels.push_back(yLabel);
        }

        qDebug() << "Sobre para chamar togglePointUI";
        togglePointUI(true);  // Mostra a UI de pontos
        qDebug() << "Fim da chamada togglePointUI";
    }
    qDebug() << "Fim setPointNumber";
}




void MainWindow::drawLine() {
    QPainter painter(&canvas);
    painter.setPen(QPen(currentColor, currentPenThickness));

    for (int i = 0; i < pointList.size() - 1; i++) {
        painter.drawLine(pointList[i], pointList[i + 1]);
    }
    update();
    togglePointUI(false);  // Esconde a UI de pontos
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton && pointsCreated < numberOfPointsToCreate) {
        pointList.append(event->pos());
        pointsCreated++;

        QLabel *xLabel = pointXLabels[pointsCreated - 1];
        QLabel *yLabel = pointYLabels[pointsCreated - 1];
        xLabel->setText("X" + QString::number(pointsCreated) + ": " + QString::number(event->pos().x()));
        yLabel->setText("Y" + QString::number(pointsCreated) + ": " + QString::number(event->pos().y()));

        // Define a posição e mostra o label
        xLabel->setGeometry(620, 70 + (pointsCreated - 1) * 30, 80, 20);
        yLabel->setGeometry(700, 70 + (pointsCreated - 1) * 30, 80, 20);
        xLabel->show();
        yLabel->show();

        if(pointsCreated == numberOfPointsToCreate) {
            drawLineButton->show();  // Mostra o botão de desenhar linha quando todos os pontos foram criados
        }
        update();
    }
}


void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    botaoX->setText("X: " + QString::number(event->x()));
    botaoY->setText("Y: " + QString::number(event->y()));

    QPainter painter(&canvas);
    painter.setPen(QPen(currentColor, currentPenThickness));
    painter.drawPoint(event->pos());
    update();
}

void MainWindow::openTransformDialog() {
    TransformDialog dialog(this);

    connect(&dialog, &TransformDialog::rotateRequested, this, &MainWindow::applyRotation);
    connect(&dialog, &TransformDialog::scaleRequested, this, &MainWindow::applyScale);
    connect(&dialog, &TransformDialog::translateRequested, this, &MainWindow::applyTranslation);
    connect(&dialog, &TransformDialog::reflectRequested, this, &MainWindow::applyReflection);

    dialog.exec();
}

void MainWindow::applyRotation() {
    // Criando uma QPixmap temporária para armazenar a imagem transformada
    QPixmap tempPixmap(canvas.size());
    tempPixmap.fill(Qt::transparent);  // preenche com transparente para garantir que não haja artefatos

    QPainter painter(&tempPixmap);
    painter.translate(canvas.width() / 2, canvas.height() / 2);  // movendo o ponto central da rotação para o centro da imagem
    painter.rotate(90);
    painter.translate(-canvas.width() / 2, -canvas.height() / 2);  // retornando ao canto superior esquerdo após a rotação
    painter.drawPixmap(0, 0, canvas);

    canvas = tempPixmap;  // Atualizando a imagem principal
    update();
}

void MainWindow::applyScale() {
    QPixmap tempPixmap(canvas.size());
    tempPixmap.fill(Qt::transparent);

    QPainter painter(&tempPixmap);
    painter.translate(canvas.width() / 2, canvas.height() / 2);  
    painter.scale(1.1, 1.1);
    painter.translate(-canvas.width() / 2, -canvas.height() / 2);  
    painter.drawPixmap(0, 0, canvas);

    canvas = tempPixmap;
    update();
}

void MainWindow::applyTranslation() {
    QPixmap tempPixmap(canvas.size());
    tempPixmap.fill(Qt::white); // Ou Qt::transparent, dependendo do que você quer

    QPainter painter(&tempPixmap);
    painter.drawPixmap(10, 10, canvas);  // desloca a imagem em 10 pixels para direita e para baixo

    canvas = tempPixmap;
    update();
}

void MainWindow::applyReflection() {
    QPixmap tempPixmap(canvas.size());
    tempPixmap.fill(Qt::transparent);

    QPainter painter(&tempPixmap);
    painter.translate(canvas.width(), 0);  // mover a imagem para a direita para refletir ao longo do eixo y
    painter.scale(-1, 1);  // refletir a imagem ao longo do eixo y
    painter.drawPixmap(0, 0, canvas);

    canvas = tempPixmap;
    update();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, canvas);
}

QColor MainWindow::getCurrentColor() const {
    return currentColor;
}

void MainWindow::setCurrentColor(const QColor &color) {
    currentColor = color;
}

int MainWindow::getCurrentPenThickness() const {
    return currentPenThickness;
}

void MainWindow::setCurrentPenThickness(int thickness) {
    currentPenThickness = thickness;
}

void MainWindow::clearCanvas() {
    canvas.fill(Qt::white);
    update();
}

void MainWindow::openColorPicker() {
    ::openColorPicker(this);
}

void MainWindow::adjustPenThickness() {
    bool ok;
    int thickness = QInputDialog::getInt(this, "Mudar o Tamanho do pincel", "Qual o tamanho:", getCurrentPenThickness(), 1, 50, 1, &ok);
    if (ok) {
        setCurrentPenThickness(thickness);
    }
}

