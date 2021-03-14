#pragma once
#include <memory>
#include <utility>
#include <vector>
#include <QObject>

#include "Input.h"
#include "MouseInput.h"
#include "Object.h"
#include <QTransform>


#include "Background.h"
#include "MeshLoader.h"
#include "ShaderCollection.h"
#include "ShaderData.h"
#include "VolumetricCubeMeshRenderer.h"

class Scene final
{
	std::shared_ptr<QOpenGLFunctions> funcions;
	void createLightSourceBlock()
	{
		lightSourceBlock = createObject(MeshLoader().loadModel("Assets/Models/ico.obj")[0], QVector3D(0, 0, 0),
			ShaderCollection::shaders["lightSource"]);
	}
	std::shared_ptr<Object> createObject(const MeshLoader::LoadedModel& model, const QVector3D& pos, ShaderData& data, const std::string& tag = "") const
	{
		auto object = std::make_shared<Object>();
		object->tag = tag;
		object->mesh = model.mesh;
		object->material = model.material;
		object->renderer = data.renderer->getRenderer();
		object->initRenderer(funcions, data.getShader());

		object->transform.translate(pos);
		return object;
	}
	
	
public:
	std::vector<std::shared_ptr<Object>> objects;
	std::vector<std::shared_ptr<LightSource>> lights;
	std::vector<std::shared_ptr<Object>> transparentObjects;
	std::shared_ptr<Object> lightSourceBlock;
	
	Background backround;

	GLCamera camera;

	bool drawWireframe = false;

	QVector2D angularVelocity = {0,0};
	
	Scene() = default;

	Scene(std::shared_ptr<QOpenGLFunctions> _functions): funcions(std::move(_functions))
	{
		createLightSourceBlock();

		backround = Background(funcions, "Assets\\Models\\textures\\Nebula_N0.PNG");
	}
	void addModel(const std::vector<MeshLoader::LoadedModel>& models, const QVector3D& pos, ShaderData& data, const std::string& tag = "")
	{
		for (const auto& model : models)
		{
			objects.push_back(createObject(model, pos, data, tag));
		}
	}
	void addLight(std::shared_ptr<LightSource> light)
	{
		lights.push_back(std::move(light));
	}
	void addTransparent(const std::vector<MeshLoader::LoadedModel>& models, const QVector3D& pos, ShaderData& data, const std::string& tag = "")
	{
		for (const auto& model : models)
		{
			transparentObjects.push_back(createObject(model, pos, data, tag));
		}
	}
};
