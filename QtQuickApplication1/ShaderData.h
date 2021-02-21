#pragma once
#include <string>
#include "MeshRendererGenerator.h"

struct ShaderData final
{
public:
	std::string vertex;
	std::string fragment;
	std::string geometry;

	std::shared_ptr<MeshRendererGenerator> renderer;
};
