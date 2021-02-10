#pragma once
#include "GLWindow.h"

#include <memory>

#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions>
#include <QScreen>
#include <QOpenGLExtraFunctions>
#include <array>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>


#include "GLCamera.h"

namespace
{
	std::vector<float> points = {
		 //position           normal               color
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
												   
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
												   
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
												   
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
												   
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,  0.0f,
												   
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f };
}

namespace fgl
{
	class TriangleWindow final : public GLWindow
	{
		typedef void (*PglGenVertexArrays) (GLsizei n, GLuint* arrays);
		typedef void (*PglBindVertexArray) (GLuint array);
	public:
		GLCamera camera;
		QOpenGLVertexArrayObject* m_vao = nullptr;
		QOpenGLBuffer* m_vbo = nullptr;
		QOpenGLBuffer m_vertexBuffer;

		void init() override
		{
			program_ = std::make_unique<QOpenGLShaderProgram>(this);
			program_->addShaderFromSourceFile(QOpenGLShader::Vertex,
				"Shaders/triangle.vs");
			program_->addShaderFromSourceFile(QOpenGLShader::Fragment,
				"Shaders/triangle.fs");

			auto s = program_->log();
			program_->link();
			
			GLuint VAO;
			PglGenVertexArrays glGenVertexArrays = reinterpret_cast<PglGenVertexArrays>(context_->getProcAddress("glGenVertexArrays"));
			PglBindVertexArray glBindVertexArray = reinterpret_cast<PglBindVertexArray>(context_->getProcAddress("glBindVertexArray"));
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);

			m_vertexBuffer.create();
			m_vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
			if (!m_vertexBuffer.bind())
			{
				qWarning() << "Could not bind vertex buffer to the context";
				return;
			}
			m_vertexBuffer.allocate(points.data(), points.size() * sizeof(float));

			program_->setAttributeBuffer("posAttr", GL_FLOAT, 0, 3, 9 * sizeof(float));
			program_->setAttributeBuffer("normalAttr", GL_FLOAT, 3 * sizeof(float), 3, 9 * sizeof(float));
			program_->setAttributeBuffer("colAttr", GL_FLOAT, 6 * sizeof(float), 3, 9 * sizeof(float));
			
			program_->enableAttributeArray("posAttr");
			program_->enableAttributeArray("colAttr");
			program_->enableAttributeArray("normalAttr");

		}

		void render() override
		{
			const auto retinaScale = devicePixelRatio();
			glViewport(0, 0, width() * retinaScale, height() * retinaScale);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);
			
			program_->bind();

			QMatrix4x4 model;
			model.rotate(100.0f * frame_ / screen()->refreshRate(), 0, 1, 0);

			camera.position.setZ(camera.position.z() - 0.005f);
			 
			program_->setUniformValue(program_->uniformLocation("model"), model);
			program_->setUniformValue(program_->uniformLocation("view"), camera.getViewMatrix());
			program_->setUniformValue(program_->uniformLocation("projection"), camera.getProjectionMatrix());
			program_->setUniformValue(program_->uniformLocation("cameraPos"), camera.position);
			
			glDrawArrays(GL_TRIANGLES, 0, points.size()/6);
			
			program_->release();

			++frame_;
		}

	private:
		// Shader program handler.
		std::unique_ptr<QOpenGLShaderProgram> program_ = nullptr;
		
		// Frame counter for animation.
		int frame_ = 0;
	};
} // namespace fgl
