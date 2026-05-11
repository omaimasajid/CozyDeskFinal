#include "CozyDeskFinal.h"
#include <QtWidgets/QApplication>
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    CozyDeskFinal window;

    app.setStyle(QStyleFactory::create("Fusion"));

    QPalette lightPalette;
    lightPalette.setColor(QPalette::Window, QColor(240, 240, 255));
    lightPalette.setColor(QPalette::WindowText, QColor(26, 26, 60));
    lightPalette.setColor(QPalette::Base, QColor(255, 255, 255));
    lightPalette.setColor(QPalette::Text, QColor(26, 26, 60));
    lightPalette.setColor(QPalette::Button, QColor(220, 222, 240));
    lightPalette.setColor(QPalette::ButtonText, QColor(26, 26, 60));
    lightPalette.setColor(QPalette::Highlight, QColor(127, 119, 221));
    lightPalette.setColor(QPalette::HighlightedText, QColor(255, 255, 255));
    app.setPalette(lightPalette);


    window.show();
    window.setWindowTitle("Cozy Desk");
    return app.exec();
}

