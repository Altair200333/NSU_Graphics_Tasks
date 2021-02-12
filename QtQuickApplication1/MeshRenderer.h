#pragma once
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions>
#include "Mesh.h"
#include "GLCamera.h"
#include "Transform.h"

class MeshRenderer final
{
public:
	QOpenGLBuffer* vbo;
	QOpenGLBuffer* ibo;
	QOpenGLVertexArrayObject* vao;
	std::shared_ptr<QOpenGLShaderProgram> shader = nullptr;
	Transform* transform;
	Mesh* mesh = nullptr;

	MeshRenderer() = default;

	void enableAttributes()
	{
		shader->enableAttributeArray("posAttr");
		shader->setAttributeBuffer("posAttr", GL_FLOAT, 0, 3, 9 * sizeof(float));

		shader->enableAttributeArray("normalAttr");
		shader->setAttributeBuffer("normalAttr", GL_FLOAT, 3 * sizeof(float), 3, 9 * sizeof(float));
		
		shader->enableAttributeArray("colAttr");
		shader->setAttributeBuffer("colAttr", GL_FLOAT, 6 * sizeof(float), 3, 9 * sizeof(float));
	}

	MeshRenderer(QObject* parent, Transform* _transform, Mesh* _mesh, const std::string& fragment = "Shaders/triangle.fs", const std::string& vertex = "Shaders/triangle.vs"):
		mesh(_mesh), transform(_transform)
	{
		
		shader = std::make_shared<QOpenGLShaderProgram>(parent);
		shader->addShaderFromSourceFile(QOpenGLShader::Vertex,
			"Shaders/triangle.vs");
		shader->addShaderFromSourceFile(QOpenGLShader::Fragment,
			"Shaders/triangle.fs");

		auto s = shader->log();
		shader->link();
		
		vao = new QOpenGLVertexArrayObject(parent);
		vao->create();
		vao->bind();

		vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
		vbo->create();
		vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
		if (!vbo->bind())
		{
			qWarning() << "Could not bind vertex buffer to the context";
			return;
		}
		vbo->allocate(mesh->vertices.data(), mesh->vertices.size() * sizeof(Vertex));

		ibo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
		ibo->create();
		ibo->bind();
		ibo->setUsagePattern(QOpenGLBuffer::StaticDraw);
		ibo->allocate(mesh->indices.data(), mesh->indices.size() * sizeof(GLuint));

		enableAttributes();

		vao->release();
		shader->release();
	}
	int frame_ = 0;
	
	void render(GLCamera& camera)
	{
		shader->bind();

		shader->setUniformValue(shader->uniformLocation("model"), transform->transform);
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
