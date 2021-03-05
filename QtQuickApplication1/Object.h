#pragma once
#include "Material.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "SimpleMeshRenderer.h"
#include "Transform.h"

class Object
{
public:
	std::string tag;
	Transform transform;
	Mesh mesh;
	std::shared_ptr<MeshRenderer> renderer;
	Material material;
	
	void initRenderer(std::shared_ptr<QOpenGLFunctions> functions, const std::string& fragment = "Shaders/triangle.fs", const std::string& vertex = "Shaders/triangle.vs",
		const std::string& geometry = "")
	{
		renderer->initMeshRenderer(functions, &transform, &mesh, &material, fragment, vertex, geometry);
	}
	void initRenderer(std::shared_ptr<QOpenGLFunctions> functions, std::shared_ptr<QOpenGLShaderProgram> shader)
	{
		renderer->initMeshRenderer(functions, &transform, &mesh, &material, shader);
	}
	virtual ~Object() = default;
};
