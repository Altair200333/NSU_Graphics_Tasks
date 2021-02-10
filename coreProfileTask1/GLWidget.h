#pragma once
#include <QGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class GLWidget: public QGLWidget
{
Q_OBJECT
public:
    GLWidget(const QGLFormat& format, QWidget* parent = 0);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void keyPressEvent(QKeyEvent* e) override;

private:
    bool prepareShaderProgram(const QString& vertexShaderPath,
        const QString& fragmentShaderPath);

    QOpenGLShaderProgram m_shader;
    QOpenGLBuffer m_vertexBuffer;
};
