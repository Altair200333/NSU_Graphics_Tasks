#pragma once
#include <QColor>
#include <QOpenGLShaderProgram>

class Material final
{
public:
	QColor color = QColor(255, 149, 100);
	bool isLightSource = false;
	enum ShadingMode : int
	{
		vertexColor = 1,
		materialColor = 2
	};
	ShadingMode shadingMode = materialColor;
	
	void uploadToShader(std::shared_ptr<QOpenGLShaderProgram>& shader)
	{
		shader->setUniformValue("mode", shadingMode);

		if(shadingMode == materialColor)
		{
			shader->setUniformValue("color", color);
		}
	}
};
