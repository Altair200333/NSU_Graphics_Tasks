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
	
	void initRenderer(std::shared_ptr<QObject> parent, std::shared_ptr<QOpenGLFunctions> functions, const std::string& fragment = "Shaders/triangle.fs", const std::string& vertex = "Shaders/triangle.vs",
		const std::string& geometry = "")
	{
		renderer->init(parent, functions, &transform, &mesh, &material, fragment, vertex, geometry);
	}
	virtual ~Object() = default;
};
