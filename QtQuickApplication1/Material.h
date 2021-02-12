#pragma once
#include <QColor>
#include <QOpenGLShaderProgram>

class Material final
{
public:
	QColor color = QColor(255, 149, 100);

	enum Mode : int
	{
		vertexColor = 1,
		materialColor = 2
	};
	Mode mode = materialColor;
	
	void uploadToShader(std::shared_ptr<QOpenGLShaderProgram>& shader)
	{
		shader->setUniformValue("mode", mode);

		if(mode == materialColor)
		{
			shader->setUniformValue("color", color);
		}
	}
};
