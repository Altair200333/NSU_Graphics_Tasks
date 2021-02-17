#include <QGuiApplication>
#include <QApplication>
#include "DemoWindow.h"
#include <QLabel>
#include "QColorDialog"
int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    QSurfaceFormat format;
    format.setSamples(16);
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);

    DemoWindow window;
    window.setFormat(format);
    window.resize(1200, 800);
    window.show();
  
    return app.exec();
}