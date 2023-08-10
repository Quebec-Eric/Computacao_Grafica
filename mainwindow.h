#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
#include <QLabel>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap canvas;
    QColor currentColor;
    QLabel *xLabel;
    QLabel *yLabel;

private slots:
    void setColorRed();
    void setColorBlue();
    void setColorGreen();
    void clearCanvas();
};

#endif // MAINWINDOW_H
