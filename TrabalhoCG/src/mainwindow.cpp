#include "mainwindow.h"
#include "ModificacoesQuadro/changeColor.h"
#include "ModificacoesQuadro/changeThickness.h"
#include "ModificacoesQuadro/clearCanvas.h"
#include "TransformDialog.h"
#include <QColorDialog>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), currentColor(Qt::black), currentPenThickness(5) 
{
    resize(800, 600);
    this->setStyleSheet("background-color: #2E2E2E;");

    //criar o local para desehar
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

    // Se você tiver mais botões, continue o padrão acima, ajustando as coordenadas y para posicioná-los corretamente.
}


void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        QPainter painter(&canvas);
        painter.setPen(QPen(currentColor, currentPenThickness));
        painter.drawPoint(event->pos());
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
    // Seu código para aplicar rotação ao desenho
    // Para este exemplo, vou simplesmente rodar o canvas em 90 graus
    QTransform transform;
    transform.rotate(90);
    canvas = canvas.transformed(transform);
    update();
}

void MainWindow::applyScale() {
    // Seu código para aplicar escala ao desenho
    // Exemplo: Aumentar o tamanho em 10%
    QTransform transform;
    transform.scale(1.1, 1.1);

    // Crie um QPixmap temporário com a mesma dimensão do canvas original
    QPixmap tempPixmap(600, 600);
    tempPixmap.fill(Qt::white);  // Preenche com branco

    // Desenha a imagem escalada no QPixmap temporário
    QPainter painter(&tempPixmap);
    painter.setTransform(transform);
    painter.drawPixmap(0, 0, canvas);

    // Atualize o canvas principal com a imagem escalada
    canvas = tempPixmap;

    update();
}


void MainWindow::applyTranslation() {
    // Seu código para transladar o desenho
    // Exemplo: Mover 10 pixels para a direita e 10 para baixo
    QTransform transform;
    transform.translate(10, 10);
    canvas = canvas.transformed(transform);
    update();
}

void MainWindow::applyReflection() {
    // Seu código para refletir o desenho
    // Exemplo: Refletir horizontalmente
    QTransform transform;
    transform.scale(-1, 1);
    canvas = canvas.transformed(transform);
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
