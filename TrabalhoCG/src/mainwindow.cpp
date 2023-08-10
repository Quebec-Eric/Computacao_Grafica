#include "mainwindow.h"
#include "ModificacoesQuadro/changeColor.h"
#include "ModificacoesQuadro/changeThickness.h"
#include "ModificacoesQuadro/clearCanvas.h"
#include <QColorDialog>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), currentColor(Qt::black), currentPenThickness(5) 
{
    setFixedSize(800, 600);

    canvas = QPixmap(600, 600);
    canvas.fill(Qt::white);

    xLabel = new QLabel(this);
    xLabel->setGeometry(620, 10, 160, 20);

    yLabel = new QLabel(this);
    yLabel->setGeometry(620, 40, 160, 20);

    QPushButton *colorPickerButton = new QPushButton("Cor", this);
    colorPickerButton->setGeometry(620, 70, 160, 30);
    connect(colorPickerButton, &QPushButton::clicked, this, &MainWindow::openColorPicker);

    QPushButton *adjustThicknessButton = new QPushButton("Mudar Tamanho", this);
    adjustThicknessButton->setGeometry(620, 110, 160, 30);
    connect(adjustThicknessButton, &QPushButton::clicked, this, &MainWindow::adjustPenThickness);

    QPushButton *clearButton = new QPushButton("Clear", this);
    clearButton->setGeometry(620, 150, 160, 30);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::clearCanvas);
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
    xLabel->setText("X: " + QString::number(event->x()));
    yLabel->setText("Y: " + QString::number(event->y()));

    QPainter painter(&canvas);
    painter.setPen(QPen(currentColor, currentPenThickness));
    painter.drawPoint(event->pos());
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
    // This is just a basic example. You may want a more sophisticated UI to adjust the thickness.
    bool ok;
    int thickness = QInputDialog::getInt(this, "Adjust Thickness", "Enter pen thickness:", getCurrentPenThickness(), 1, 50, 1, &ok);
    if (ok) {
        setCurrentPenThickness(thickness);
    }
}
