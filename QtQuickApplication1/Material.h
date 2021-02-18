#pragma once
#include <QColor>
#include <QOpenGLShaderProgram>

#include "Texture.h"

class Material final
{
public:
	std::vector<Texture> diffuse;
	std::vector<Texture> normal;
	
	QColor color;
	bool isLightSource = false;
	
	enum ShadingMode : int
	{
		vertexColor = 1,
		materialColor = 2
	};
	ShadingMode shadingMode = materialColor;

	Material(QColor _color = QColor(255, 149, 100)):color(_color)
	{}
	void uploadToShader(std::shared_ptr<QOpenGLShaderProgram>& shader)
	{
		shader->setUniformValue("mode", shadingMode);

		if(shadingMode == materialColor)
		{
			shader->setUniformValue("color", color);
		}
	}
};
