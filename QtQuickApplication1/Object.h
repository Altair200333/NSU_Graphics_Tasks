#pragma once
#include "Material.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "Transform.h"

class Object
{
public:
	Transform transform;
	Mesh mesh;
	MeshRenderer renderer;
	Material material;
	
	void initRenderer(QObject* parent, const std::string& fragment = "Shaders/triangle.fs", const std::string& vertex = "Shaders/triangle.vs")
	{
		renderer = MeshRenderer(parent, &transform, &mesh, &material);
	}
	virtual ~Object() = default;
};
