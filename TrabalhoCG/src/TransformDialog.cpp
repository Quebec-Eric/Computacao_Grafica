#include "TransformDialog.h"

TransformDialog::TransformDialog(QWidget *parent)
    : QDialog(parent) 
{
    QVBoxLayout *layout = new QVBoxLayout();

    QPushButton *rotateButton = new QPushButton("Rotação", this);
    QPushButton *scaleButton = new QPushButton("Escala", this);
    QPushButton *translateButton = new QPushButton("Translação", this);
    QPushButton *reflectButton = new QPushButton("Reflexão", this);

    connect(rotateButton, &QPushButton::clicked, this, &TransformDialog::rotateRequested);
    connect(scaleButton, &QPushButton::clicked, this, &TransformDialog::scaleRequested);
    connect(translateButton, &QPushButton::clicked, this, &TransformDialog::translateRequested);
    connect(reflectButton, &QPushButton::clicked, this, &TransformDialog::reflectRequested);

    layout->addWidget(rotateButton);
    layout->addWidget(scaleButton);
    layout->addWidget(translateButton);
    layout->addWidget(reflectButton);

    setLayout(layout);
}
