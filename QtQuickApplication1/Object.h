#pragma once
#include "Material.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "SimpleMeshRenderer.h"
#include "Transform.h"

class Object
{
public:
	Transform transform;
	Mesh mesh;
	std::shared_ptr<MeshRenderer> renderer;
	Material material;
	
	void initRenderer(std::shared_ptr<QObject> parent, const std::string& fragment = "Shaders/triangle.fs", const std::string& vertex = "Shaders/triangle.vs")
	{
		renderer->init(parent, &transform, &mesh, &material);
	}
	virtual ~Object() = default;
};
