#include <QGuiApplication>
#include <QApplication>
#include "GLWindow.h"
#include <QLabel>

#include "AloyApplication.h"
#include "QColorDialog"
int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    QSurfaceFormat format;
    format.setSamples(16);
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);

    Window window;
    window.setFormat(format);
    window.resize(1600, 800);
    AloyApplication aloyApp(&window);
    window.show();
  
    return app.exec();
}