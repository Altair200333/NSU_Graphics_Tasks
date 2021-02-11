#pragma once
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions>
#include "Mesh.h"
#include "GLCamera.h"
class MeshRenderer final
{
public:
	QOpenGLBuffer* vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	QOpenGLBuffer* ibo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
	QOpenGLVertexArrayObject* vao = new QOpenGLVertexArrayObject();
	std::shared_ptr<QOpenGLShaderProgram> shader = nullptr;
	Mesh* mesh = nullptr;

	MeshRenderer() = default;
	
	MeshRenderer(QObject* parent, Mesh* _mesh, const std::string& fragment = "Shaders/triangle.fs", const std::string& vertex = "Shaders/triangle.vs"):
		mesh(_mesh)
	{
		shader = std::make_shared<QOpenGLShaderProgram>(parent);
		shader->addShaderFromSourceFile(QOpenGLShader::Vertex,
			"Shaders/triangle.vs");
		shader->addShaderFromSourceFile(QOpenGLShader::Fragment,
			"Shaders/triangle.fs");

		auto s = shader->log();
		shader->link();

		vao->create();
		vao->bind();

		vbo->create();
		vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
		if (!vbo->bind())
		{
			qWarning() << "Could not bind vertex buffer to the context";
			return;
		}
		vbo->allocate(mesh->vertices.data(), mesh->vertices.size() * sizeof(Vertex));

		ibo->create();
		ibo->bind();
		ibo->setUsagePattern(QOpenGLBuffer::StaticDraw);
		ibo->allocate(mesh->indices.data(), mesh->indices.size() * sizeof(GLuint));

		shader->setAttributeBuffer("posAttr", GL_FLOAT, 0, 3, 9 * sizeof(float));
		shader->setAttributeBuffer("normalAttr", GL_FLOAT, 3 * sizeof(float), 3, 9 * sizeof(float));
		shader->setAttributeBuffer("colAttr", GL_FLOAT, 6 * sizeof(float), 3, 9 * sizeof(float));

		shader->enableAttributeArray("posAttr");
		shader->enableAttributeArray("colAttr");
		shader->enableAttributeArray("normalAttr");

		vao->release();
	}
	int frame_ = 0;
	
	void render(GLCamera& camera)
	{
		shader->bind();

		QMatrix4x4 model;
		model.rotate(1.0f * frame_, 0, 1, 0);

		shader->setUniformValue(shader->uniformLocation("model"), model);
		shader->setUniformValue(shader->uniformLocation("view"), camera.getViewMatrix());
		shader->setUniformValue(shader->uniformLocation("projection"), camera.getProjectionMatrix());
		shader->setUniformValue(shader->uniformLocation("cameraPos"), camera.position);

		vao->bind();
		glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
		vao->release();

		shader->release();

		++frame_;
	}
};
