#pragma once
#include <map>

#include "ShaderData.h"
#include "VolumetricCubeMeshRenderer.h"

class ShaderCollection final
{

	class SimpleMeshRendererGenerator final : public MeshRendererGenerator
	{
	public:
		std::shared_ptr<MeshRenderer> getRenderer() override
		{
			return std::make_shared<SimpleMeshRenderer>();
		}
	};
	class VolumetricCubeMeshRendererGenerator final : public MeshRendererGenerator
	{
	public:
		std::shared_ptr<MeshRenderer> getRenderer() override
		{
			return std::make_shared<VolumetricCubeMeshRenderer>();
		}
	};

public:
	inline static std::map<std::string, ShaderData> shaders =
	{
		{{"pbr"}, {"Shaders\\triangle.vs","Shaders\\triangle.fs", "", std::make_shared<SimpleMeshRendererGenerator>()}},
		{{"simple"}, {"Shaders\\simple.vs","Shaders\\simple.fs", "", std::make_shared<SimpleMeshRendererGenerator>()}},
		{{"lightSource"}, {"Shaders\\lightSource.vs","Shaders\\lightSource.fs", "", std::make_shared<SimpleMeshRendererGenerator>()}},
		{{"basic_geometry"}, {"Shaders\\triangleG.vs","Shaders\\triangleG.fs","Shaders\\triangleG.gs", std::make_shared<SimpleMeshRendererGenerator>()}},
		{{"cubicCloud"}, {"Shaders\\cloud.vs","Shaders\\cloud.fs", "", std::make_shared<VolumetricCubeMeshRendererGenerator>()}},
		{{"fractal"}, {"Shaders\\fractal.vs","Shaders\\fractal.fs", "", std::make_shared<SimpleMeshRendererGenerator>()}},
	};

};
