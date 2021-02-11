#include <QGuiApplication>
#include "TriangleWindow.h"

int main(int argc, char** argv) {
    QGuiApplication app(argc, argv);

    QSurfaceFormat format;
    format.setSamples(16);
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
	
    fgl::TriangleWindow window;
    window.setFormat(format);
    window.resize(640, 480);
    window.show();

    window.setAnimated(true);

    return app.exec();
}