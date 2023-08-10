#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), currentColor(Qt::black)
{
    setFixedSize(800, 600);

    canvas = QPixmap(600, 600);
    canvas.fill(Qt::white);

    xLabel = new QLabel(this);
    xLabel->setGeometry(620, 10, 160, 20);

    yLabel = new QLabel(this);
    yLabel->setGeometry(620, 40, 160, 20);

    QPushButton *redButton = new QPushButton("Red", this);
    redButton->setGeometry(620, 70, 160, 30);
    connect(redButton, &QPushButton::clicked, this, &MainWindow::setColorRed);

    QPushButton *greenButton = new QPushButton("Green", this);
    greenButton->setGeometry(620, 110, 160, 30);
    connect(greenButton, &QPushButton::clicked, this, &MainWindow::setColorGreen);

    QPushButton *blueButton = new QPushButton("Blue", this);
    blueButton->setGeometry(620, 150, 160, 30);
    connect(blueButton, &QPushButton::clicked, this, &MainWindow::setColorBlue);

    QPushButton *clearButton = new QPushButton("Clear", this);
    clearButton->setGeometry(620, 190, 160, 30);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::clearCanvas);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        QPainter painter(&canvas);
        painter.setPen(currentColor);
        painter.drawPoint(event->pos());
        update();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    xLabel->setText("X: " + QString::number(event->x()));
    yLabel->setText("Y: " + QString::number(event->y()));

    QPainter painter(&canvas);
    painter.setPen(currentColor);
    painter.drawPoint(event->pos());
    update();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, canvas);
}

void MainWindow::setColorRed()
{
    currentColor = Qt::red;
}

void MainWindow::setColorBlue()
{
    currentColor = Qt::blue;
}

void MainWindow::setColorGreen()
{
    currentColor = Qt::green;
}

void MainWindow::clearCanvas()
{
    canvas.fill(Qt::white);
    update();
}
