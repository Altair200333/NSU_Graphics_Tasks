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

class MeshRenderer
{
public:
	virtual ~MeshRenderer() = default;
	QOpenGLBuffer* vbo = nullptr;
	QOpenGLBuffer* ibo = nullptr;
	QOpenGLVertexArrayObject* vao = nullptr;
	std::shared_ptr<QOpenGLShaderProgram> shader = nullptr;
	Transform* transform = nullptr;
	Mesh* mesh = nullptr;
	Material* material = nullptr;

	MeshRenderer() = default;

	void enableAttributes() const
	{
		shader->enableAttributeArray("posAttr");
		shader->setAttributeBuffer("posAttr", GL_FLOAT, 0, 3, sizeof(Vertex));

		shader->enableAttributeArray("normalAttr");
		shader->setAttributeBuffer("normalAttr", GL_FLOAT, offsetof(Vertex, normal), 3, sizeof(Vertex));

		shader->enableAttributeArray("colAttr");
		shader->setAttributeBuffer("colAttr", GL_FLOAT, offsetof(Vertex, color), 3, sizeof(Vertex));

		shader->enableAttributeArray("aTexCoords");
		shader->setAttributeBuffer("aTexCoords", GL_FLOAT, offsetof(Vertex, TexCoords), 2, sizeof(Vertex));

		shader->enableAttributeArray("aTangent");
		shader->setAttributeBuffer("aTangent", GL_FLOAT, offsetof(Vertex, TexCoords), 3, sizeof(Vertex));

		shader->enableAttributeArray("aBitangent");
		shader->setAttributeBuffer("aBitangent", GL_FLOAT, offsetof(Vertex, TexCoords), 3, sizeof(Vertex));
	}

	void createShader(QObject* parent, const std::string& fragment, const std::string& vertex, const std::string& geometry)
	{
		shader = std::make_shared<QOpenGLShaderProgram>(parent);
		shader->addShaderFromSourceFile(QOpenGLShader::Vertex, vertex.c_str());
		shader->addShaderFromSourceFile(QOpenGLShader::Fragment, fragment.c_str());
		
		if(!geometry.empty())
			shader->addShaderFromSourceFile(QOpenGLShader::Geometry, geometry.c_str());
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
	
	void init(std::shared_ptr<QObject> parent, Transform* _transform, Mesh* _mesh, Material* _material,
		const std::string& fragment = "Shaders/triangle.fs",
		const std::string& vertex = "Shaders/triangle.vs", const std::string& geometry = "")
	{
		mesh = _mesh;
		transform = _transform;
		material = _material;

		createShader(parent.get(), fragment, vertex, geometry);

		auto s = shader->log();
		shader->link();

		createVao(parent.get());

		createVbo();

		createIbo();

		enableAttributes();

		vao->release();
		shader->release();
	}
	
	void uploadCameraDetails(GLCamera& camera) const
	{
		shader->setUniformValue(shader->uniformLocation("model"), transform->transform);
		shader->setUniformValue(shader->uniformLocation("view"), camera.getViewMatrix());
		shader->setUniformValue(shader->uniformLocation("projection"), camera.getProjectionMatrix());
		shader->setUniformValue(shader->uniformLocation("cameraPos"), camera.position);
	}

	virtual void render(GLCamera& camera, const std::vector<std::shared_ptr<LightSource>>& lights = std::vector<std::shared_ptr<LightSource>>{}) = 0;
};
