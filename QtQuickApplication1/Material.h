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
	float roughness = 0.02f;
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
		shader->setUniformValue("roughness", roughness);

		if(shadingMode == materialColor)
		{
			shader->setUniformValue("color", color);
		}
	}
};
