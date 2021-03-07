#pragma once
#include <memory>
#include <QOpenGLShaderProgram>

class LightSource
{
public:
	QVector3D position = { 0,0,0 };
	QColor color = { 250,250,250 };

	LightSource(QVector3D _position = { 0,0,0 }, QColor _color = { 250,250,250 }) :position(_position), color(_color)
	{
	}
	virtual ~LightSource() = default;

	virtual void uploadToShader(std::shared_ptr<QOpenGLShaderProgram> shader, size_t index) = 0;
};
