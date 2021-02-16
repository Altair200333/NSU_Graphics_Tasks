#pragma once
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions>
#include "Mesh.h"
#include "GLCamera.h"
#include "LightSource.h"
#include "Material.h"
#include "Transform.h"

class MeshRenderer final
{
public:
	QOpenGLBuffer* vbo;
	QOpenGLBuffer* ibo;
	QOpenGLVertexArrayObject* vao;
	std::shared_ptr<QOpenGLShaderProgram> shader;
	Transform* transform;
	Mesh* mesh;
	Material* material;

	MeshRenderer() = default;

	void enableAttributes()
	{
		shader->enableAttributeArray("posAttr");
		shader->setAttributeBuffer("posAttr", GL_FLOAT, 0, 3, sizeof(Vertex));

		shader->enableAttributeArray("normalAttr");
		shader->setAttributeBuffer("normalAttr", GL_FLOAT, 3 * sizeof(float), 3, sizeof(Vertex));

		shader->enableAttributeArray("colAttr");
		shader->setAttributeBuffer("colAttr", GL_FLOAT, 6 * sizeof(float), 3, sizeof(Vertex));
	}

	void createShader(QObject* parent, const std::string& fragment, const std::string& vertex)
	{
		shader = std::make_shared<QOpenGLShaderProgram>(parent);
		shader->addShaderFromSourceFile(QOpenGLShader::Vertex, vertex.c_str());
		shader->addShaderFromSourceFile(QOpenGLShader::Fragment, fragment.c_str());
	}

	void createVao(QObject* parent)
	{
		vao = new QOpenGLVertexArrayObject(parent);
		vao->create();
		vao->bind();
	}

	void createVbo()
	{
		vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
		vbo->create();
		vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
		if (!vbo->bind())
		{
			qWarning() << "Could not bind vertex buffer to the context";
			return;
		}
		vbo->allocate(mesh->vertices.data(), mesh->vertices.size() * sizeof(Vertex));
	}

	void createIbo()
	{
		ibo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
		ibo->create();
		ibo->setUsagePattern(QOpenGLBuffer::StaticDraw);
		ibo->bind();
		ibo->allocate(mesh->indices.data(), mesh->indices.size() * sizeof(GLuint));
	}

	MeshRenderer(QObject* parent, Transform* _transform, Mesh* _mesh, Material* _material,
	             const std::string& fragment = "Shaders/triangle.fs",
	             const std::string& vertex = "Shaders/triangle.vs"):
		mesh(_mesh), transform(_transform), material(_material)
	{
		createShader(parent, fragment, vertex);

		auto s = shader->log();
		shader->link();

		createVao(parent);

		createVbo();

		createIbo();

		enableAttributes();

		vao->release();
		shader->release();
	}

	int frame_ = 0;

	void uploadCameraDetails(GLCamera& camera)
	{
		shader->setUniformValue(shader->uniformLocation("model"), transform->transform);
		shader->setUniformValue(shader->uniformLocation("view"), camera.getViewMatrix());
		shader->setUniformValue(shader->uniformLocation("projection"), camera.getProjectionMatrix());
		shader->setUniformValue(shader->uniformLocation("cameraPos"), camera.position);
	}

	void uploadLights(std::vector<std::shared_ptr<LightSource>>& lights)
	{
		shader->setUniformValue("lightsCount", static_cast<int>(lights.size()));
		for (int i = 0; i < lights.size(); ++i)
		{
			shader->setUniformValue(("lights[" + std::to_string(i) + "].position").c_str(), lights[i]->position);
			shader->setUniformValue(("lights[" + std::to_string(i) + "].color").c_str(), lights[i]->color);
		}
	}

	void render(GLCamera& camera, std::vector<std::shared_ptr<LightSource>>& lights)
	{
		shader->bind();

		material->uploadToShader(shader);
		uploadCameraDetails(camera);
		uploadLights(lights);
		shader->setUniformValue("isLightSource", material->isLightSource);
		
		vao->bind();
		glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
		vao->release();

		shader->release();

		++frame_;
	}
};
