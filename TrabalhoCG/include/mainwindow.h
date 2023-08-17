#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QLabel>
#include "TransformDialog.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    QColor getCurrentColor() const;
    void setCurrentColor(const QColor &color);

    int getCurrentPenThickness() const;
    void setCurrentPenThickness(int thickness);

    void clearCanvas();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private slots:
    void openColorPicker();
    void adjustPenThickness();
    void openTransformDialog();
    void applyRotation();
    void applyScale();
    void applyTranslation();
    void applyReflection();
    void setPointNumber();
    void drawLine();

private:
    QPixmap canvas;
    QColor currentColor;
    int currentPenThickness;
    QList<QLabel*> pointXLabels;
    QList<QLabel*> pointYLabels;

    QLabel *botaoX;
    QLabel *botaoY;
    QPushButton *graphButton;
    QPushButton *drawLineButton;  // botão para desenhar a linha

    int numberOfPointsToCreate;
    int pointsCreated;
    QVector<QPoint> pointList;

    void initializeUI();  // Função para inicializar a interface
    void togglePointUI(bool show);  // Função para alternar entre UIs
};

#endif // MAINWINDOW_H
