#pragma once
#include "Mesh.h"
#include "MeshRenderer.h"
#include "Transform.h"

class Object final
{
public:
	Transform transform;
	Mesh mesh;
	MeshRenderer renderer;

	void initRenderer(QObject* parent, const std::string& fragment = "Shaders/triangle.fs", const std::string& vertex = "Shaders/triangle.vs")
	{
		renderer = MeshRenderer(parent, &transform, &mesh);
	}
};
