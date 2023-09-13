#include "mainwindow.h"
#include "ModificacoesQuadro/changeColor.h"
#include "ModificacoesQuadro/changeThickness.h"
#include "ModificacoesQuadro/clearCanvas.h"
#include "TransformDialog.h"
#include <QColorDialog>
#include <QInputDialog>
#include <QDebug>
#include <QtGlobal>
#include <cstdlib>
#include <QRadioButton>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), currentColor(Qt::black), currentPenThickness(5)
{
    resize(800, 600);
    this->setStyleSheet("background-color: #2E2E2E;");

    initializeUI();
}

void MainWindow::initializeUI()
{

    canvas = QPixmap(600, 600);
    canvas.fill(Qt::white);
    // criar botao X
    botaoX = new QLabel(this);
    botaoX->setGeometry(620, 10, 160, 20);

    // criar botao y
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
    int buttonRow2Y = 400 + 2 * (buttonHeight + buttonSpacing);
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
    pointButton->setGeometry(620, buttonRow2Y, buttonWidth, buttonHeight); // Mudei o y para buttonRow2Y
    connect(pointButton, &QPushButton::clicked, this, &MainWindow::fazerBotoes);

    drawLineButton = new QPushButton("Desenhar Reta", this);
    drawLineButton->setGeometry(620 + buttonWidth + buttonSpacing, buttonRow2Y, buttonWidth, buttonHeight); // Mudei o y para buttonRow2Y
    connect(drawLineButton, &QPushButton::clicked, this, &MainWindow::showAlgorithmDialog);                 // Mudou para showAlgorithmDialog
    drawLineButton->hide();

    int buttonRow3Y = buttonRow2Y + buttonHeight + buttonSpacing;

    fazerJanelaButton = new QPushButton("Fazer Janela", this);
    fazerJanelaButton->setGeometry(620, buttonRow3Y, buttonWidth, buttonHeight);       // Definir posição e tamanho
    connect(fazerJanelaButton, &QPushButton::clicked, this, &MainWindow::fazerJanela); // Conectar com o slot fazerJanela
    fazerJanelaButton->hide();
}

void MainWindow::fazerJanela()
{
    
    
      int canvasWidth = canvas.width();
    int canvasHeight = canvas.height();

    
    QPoint rectTopLeft(canvasWidth / 6, canvasHeight / 4);  
    QPoint rectBottomRight(5 * canvasWidth / 6, 3 * canvasHeight / 4); 

    
    QPainter painter(&canvas);

    
    painter.setPen(QPen(Qt::red, 2));

    painter.drawRect(QRect(rectTopLeft, rectBottomRight));
    painter.end();  
    update();


    QDialog dialogJanela(this);
    dialogJanela.setWindowTitle("Qual algoritmo de corte");
    QVBoxLayout layout;
    dialogJanela.setLayout(&layout);
    QRadioButton cohen("Cohen");
    QRadioButton liang("Liang");

    layout.addWidget(&cohen);
    layout.addWidget(&liang);

    QPushButton ok("Ok");
    connect(&ok, &QPushButton::clicked, [&]
            {
                if (cohen.isChecked()) {
                    
                }
                if (liang.isChecked()) {
                    
                }
                
                dialogJanela.accept(); 
            });

    layout.addWidget(&ok);
    dialogJanela.exec();

}



void MainWindow::showAlgorithmDialog()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Escolher Algoritmo para Desenhar Linha");

    QVBoxLayout layout;
    dialog.setLayout(&layout);

    QRadioButton bresenham("Bresenham");
    QRadioButton dda("DDA");
    QRadioButton circulo("Circulo");

    layout.addWidget(&bresenham);
    layout.addWidget(&dda);
    layout.addWidget(&circulo);

    QPushButton ok("Ok");
    connect(&ok, &QPushButton::clicked, [&]
            {
        if (bresenham.isChecked()) {
            drawLineRR();
        }
        if (dda.isChecked()) {
            drawLine();  
        }
        if(circulo.isChecked()){
            drawCircle();
        }
        dialog.accept(); });

    layout.addWidget(&ok);
    dialog.exec();
}

void MainWindow::togglePointUI(bool show)
{

    if (!botaoX || !botaoY || !graphButton || !drawLineButton)
    {

        return;
    }

    botaoX->setVisible(show);
    botaoY->setVisible(show);
    graphButton->setVisible(show);
    drawLineButton->setVisible(show);
}

void MainWindow::fazerBotoes()
{

    bool ok;
    int numPoints = QInputDialog::getInt(this, "Número de Pontos", "Quantos pontos você quer criar?", 1, 1, 50, 1, &ok);
    if (ok)
    {

        numberOfPointsToCreate = numPoints;
        pointsCreated = 0;
        pointList.clear();

        for (auto &label : pointXLabels)
        {
            delete label;
        }
        for (auto &label : pointYLabels)
        {
            delete label;
        }
        pointXLabels.clear();
        pointYLabels.clear();

        for (int i = 0; i < numPoints; i++)
        {
            QLabel *xLabel = new QLabel(this);
            QLabel *yLabel = new QLabel(this);
            xLabel->hide();
            yLabel->hide();
            pointXLabels.push_back(xLabel);
            pointYLabels.push_back(yLabel);
        }

        togglePointUI(true);
    }
}

void MainWindow::drawLineRR()
{
    if (pointList.size() < 2)
        return;

    for (int i = 0; i < pointList.size() - 1; i++)
    {
        fazerBres(pointList[i].x(), pointList[i].y(), pointList[i + 1].x(), pointList[i + 1].y());
    }
    update();
    togglePointUI(false);
    fazerJanelaButton->show();
}

void MainWindow::drawLine()
{
    if (pointList.size() < 2)
        return;

    for (int i = 0; i < pointList.size() - 1; i++)
    {
        fazerDDA(pointList[i].x(), pointList[i].y(), pointList[i + 1].x(), pointList[i + 1].y());
    }
    update();
    togglePointUI(false);
    fazerJanelaButton->show();
}

void MainWindow::drawCircle()
{
    if (pointList.size() < 2)
        return;

    int x1 = pointList[0].x();
    int y1 = pointList[0].y();
    int x2 = pointList[1].x();
    int y2 = pointList[1].y();

    int r = std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));

    fazerCirculo(x1, y1, r);

    update();
    togglePointUI(false);
}

void MainWindow::fazerDDA(int x1, int y1, int x2, int y2)
{
    QPainter painter(&canvas);
    painter.setPen(QPen(currentColor, currentPenThickness));

    int deltaX = x2 - x1;
    int deltaY = y2 - y1;
    float X = x1;
    float Y = y1;

    int quantidadeEtapas = abs(deltaX) > abs(deltaY) ? abs(deltaX) : abs(deltaY);
    float Xant = deltaX / (float)quantidadeEtapas;
    float Yant = deltaY / (float)quantidadeEtapas;

    for (int i = 0; i <= quantidadeEtapas; i++)
    {
        painter.drawPoint(std::round(X), std::round(Y));
        X += Xant;
        Y += Yant;
    }

    painter.end();
}

void MainWindow::fazerBres(int x1, int y1, int x2, int y2)
{
    QPainter painter(&canvas);
    painter.setPen(QPen(currentColor, currentPenThickness));

    int encrementrarX = 0;
    int encrementrarY = 0;
    int deltaX = (x2 - x1);
    int deltaY = (y2 - y1);
    int constante1 = 0;
    int constante2 = 0;
    int p = 0;

    if (deltaX >= 0)
    {
        encrementrarX = 1;
    }
    else
    {
        encrementrarX = -1;
        deltaX = -deltaX;
    }

    if (deltaY >= 0)
    {
        encrementrarY = 1;
    }
    else
    {
        encrementrarY = -1;
        deltaY = -deltaY;
    }

    int x = x1;
    int y = y1;

    painter.drawPoint(std::round(x), std::round(y));

    if (deltaX >= deltaY)
    {
        p = 2 * deltaY - deltaX;
        constante1 = 2 * deltaY;
        constante2 = 2 * (deltaY - deltaX);
        for (int i = 0; i < deltaX; i++)
        {
            x += encrementrarX;
            if (p < 0)
            {
                p += constante1;
            }
            else
            {
                y += encrementrarY;
                p += constante2;
            }
            painter.drawPoint(std::round(x), std::round(y));
        }
    }
    else
    {
        p = 2 * deltaX - deltaY;
        constante1 = 2 * deltaX;
        constante2 = 2 * (deltaX - deltaY);
        for (int i = 0; i < deltaY; i++)
        {
            y += encrementrarY;
            if (p < 0)
            {
                p += constante1;
            }
            else
            {
                x += encrementrarX;
                p += constante2;
            }
            painter.drawPoint(std::round(x), std::round(y));
        }
    }
}

void MainWindow::fazerCirculo(int xc, int yc, int r)
{
    QPainter painter(&canvas);
    painter.setPen(QPen(currentColor, currentPenThickness));

    auto draw_circle = [&](int x, int y)
    {
        painter.drawPoint(xc + x, yc + y);
        painter.drawPoint(xc - x, yc + y);
        painter.drawPoint(xc + x, yc - y);
        painter.drawPoint(xc - x, yc - y);
        painter.drawPoint(xc + y, yc + x);
        painter.drawPoint(xc - y, yc + x);
        painter.drawPoint(xc + y, yc - x);
        painter.drawPoint(xc - y, yc - x);
    };

    int x = 0, y = r;
    int d = 3 - 2 * r;
    draw_circle(x, y);

    while (y >= x)
    {
        x++;

        if (d > 0)
        {
            y--;
            d += 4 * (x - y) + 10;
        }
        else
        {
            d += 4 * x + 6;
        }
        draw_circle(x, y);
    }

    painter.end();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (isDrawingRect)
    {
        if (event->button() == Qt::LeftButton)
        {
            rectTopLeft = event->pos();
        }
        else if (event->button() == Qt::RightButton)
        {
            rectBottomRight = event->pos();
            isDrawingRect = false;
            // Agora você pode habilitar o botão ou o menu para escolher o algoritmo
        }
    }
    else
    {
        if (event->button() == Qt::LeftButton && pointsCreated < numberOfPointsToCreate)
        {
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

            if (pointsCreated == numberOfPointsToCreate)
            {
                drawLineButton->show();
                
            }
            update();
        }
    }
    update();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    botaoX->setText("X: " + QString::number(event->x()));
    botaoY->setText("Y: " + QString::number(event->y()));

    QPainter painter(&canvas);
    painter.setPen(QPen(currentColor, currentPenThickness));
    painter.drawPoint(event->pos());
    if (isDrawingRect) {
        QPainter painter(&canvas);
        painter.setPen(Qt::DashLine); // Linha tracejada para a janela de recorte
        painter.drawRect(QRect(rectTopLeft, event->pos()));
    }
    update();
}

void MainWindow::openTransformDialog()
{
    TransformDialog dialog(this);

    connect(&dialog, &TransformDialog::rotateRequested, this, &MainWindow::fazerRotacao);
    connect(&dialog, &TransformDialog::scaleRequested, this, &MainWindow::fazerScala);
    connect(&dialog, &TransformDialog::translateRequested, this, &MainWindow::fazertrannsf);
    connect(&dialog, &TransformDialog::reflectRequested, this, &MainWindow::fazerRefle);

    dialog.exec();
}

void MainWindow::fazerRotacao()
{

    QPixmap tempPixmap(canvas.size());
    tempPixmap.fill(Qt::transparent);

    QPainter painter(&tempPixmap);
    painter.translate(canvas.width() / 2, canvas.height() / 2);
    painter.rotate(90);
    painter.translate(-canvas.width() / 2, -canvas.height() / 2);
    painter.drawPixmap(0, 0, canvas);

    canvas = tempPixmap;
    update();
}

void MainWindow::fazerScala()
{
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

void MainWindow::fazertrannsf()
{
    QPixmap tempPixmap(canvas.size());
    tempPixmap.fill(Qt::white);

    QPainter painter(&tempPixmap);
    painter.drawPixmap(10, 10, canvas);

    canvas = tempPixmap;
    update();
}

void MainWindow::fazerRefle()
{
    QPixmap tempPixmap(canvas.size());
    tempPixmap.fill(Qt::transparent);

    QPainter painter(&tempPixmap);
    painter.translate(canvas.width(), 0);
    painter.scale(-1, 1);
    painter.drawPixmap(0, 0, canvas);

    canvas = tempPixmap;
    update();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, canvas);
}

QColor MainWindow::getCurrentColor() const
{
    return currentColor;
}

void MainWindow::setCurrentColor(const QColor &color)
{
    currentColor = color;
}

int MainWindow::getCurrentPenThickness() const
{
    return currentPenThickness;
}

void MainWindow::setCurrentPenThickness(int thickness)
{
    currentPenThickness = thickness;
}

void MainWindow::clearCanvas()
{
    canvas.fill(Qt::white);
    update();
}

void MainWindow::openColorPicker()
{
    ::openColorPicker(this);
}

void MainWindow::adjustPenThickness()
{
    bool ok;
    int thickness = QInputDialog::getInt(this, "Mudar o Tamanho do pincel", "Qual o tamanho:", getCurrentPenThickness(), 1, 50, 1, &ok);
    if (ok)
    {
        setCurrentPenThickness(thickness);
    }
}
