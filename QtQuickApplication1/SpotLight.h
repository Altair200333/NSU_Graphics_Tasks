#pragma once
#include "LightSource.h"
#include <QColor>
#include <QVector3D>

class SpotLight final: public LightSource
{
public:
	QVector3D direction;
	float intensity = 1;
	SpotLight(QVector3D _position = { 0,0,0 }, QColor _color = { 250,250,250 }, QVector3D _direction = QVector3D(0, -1, 0))
	: LightSource(_position, _color), direction(_direction)
	{
	}

	void uploadToShader(std::shared_ptr<QOpenGLShaderProgram> shader, size_t index) override
	{
		shader->setUniformValue(("spotLights[" + std::to_string(index) + "].position").c_str(), position);
		shader->setUniformValue(("spotLights[" + std::to_string(index) + "].color").c_str(), color);
		shader->setUniformValue(("spotLights[" + std::to_string(index) + "].direction").c_str(), direction);
		
		shader->setUniformValue(("spotLights[" + std::to_string(index) + "].cutOff").c_str(), cos(qDegreesToRadians(12.5f)));
		shader->setUniformValue(("spotLights[" + std::to_string(index) + "].outerCutOff").c_str(), cos(qDegreesToRadians(17.5f)));
		shader->setUniformValue(("spotLights[" + std::to_string(index) + "].intensity").c_str(), intensity);
		
	}
};
