#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QLabel>
#include "TransformDialog.h"
#include <cmath>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    QColor getCurrentColor() const;
    void setCurrentColor(const QColor &color);
    void coletarDados();
    int getCurrentPenThickness() const;
    void setCurrentPenThickness(int thickness);
    void cohenfazer(const QRect &rect);
    void  liangBarskyClipping(const QRect &rect);
    void clearCanvas();
    QPoint rectTopLeft, rectBottomRight;
    bool isDrawingRect = false;
     QRect clippingRect; 
    int xMax,xMin;  
    
    int yMax,yMin;
   

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;


private slots:
    void openColorPicker();
    void adjustPenThickness();
    void openTransformDialog();
    void fazerRotacao();
    void fazerScala();
    void showAlgorithmDialog();
    void fazertrannsf();
    void fazerRefle();
    void fazerBotoes();
    void drawCircle();
    void fazerCirculo(int xc, int yc, int r);
    void drawLine();
    void fazerJanela();
    void drawLineRR();
    void fazerDDA(int x1, int y1, int x2, int y2);
    void teste(QPainter &painter, int x1, int y1, int x2, int y2);
    void fazerBres(int x1, int y1, int x2, int y2);
    void fazer();
private:
    QPixmap canvas;
    QColor currentColor;
    QPushButton *fazerJanelaButton;
    int currentPenThickness;
    QList<QLabel*> pointXLabels;
    QList<QLabel*> pointYLabels;
    QLabel *botaoX;
    QLabel *botaoY;
    QPushButton *graphButton;
    QPushButton *drawLineButton;    
    int numberOfPointsToCreate;
    int pointsCreated;
    QVector<QPoint> pointList;
    
    void initializeUI();  
    void togglePointUI(bool show); 
};

#endif // MAINWINDOW_H
