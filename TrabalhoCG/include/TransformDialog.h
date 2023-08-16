#ifndef TRANSFORMDIALOG_H
#define TRANSFORMDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>

class TransformDialog : public QDialog {
    Q_OBJECT

public:
    explicit TransformDialog(QWidget *parent = nullptr);

signals:
    void rotateRequested();
    void scaleRequested();
    void translateRequested();
    void reflectRequested();
};

#endif // TRANSFORMDIALOG_H
