#include "ModificacoesQuadro/changeColor.h"
#include <QColorDialog>
#include "../../include/mainwindow.h"

void openColorPicker(MainWindow *window) {
    QColor color = QColorDialog::getColor(window->getCurrentColor(), window);
    if (color.isValid()) {
        window->setCurrentColor(color);
    }
}
