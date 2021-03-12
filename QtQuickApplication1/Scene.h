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
		lightSourceBlock = createObject(MeshLoader().loadModel("Assets/Models/cube.obj")[0], QVector3D(0, 0, 0),
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
	void addTransparent(const std::vector<MeshLoader::LoadedModel>& models, const QVector3D& pos, ShaderData& data, const std::string& tag = "")
	{
		for (const auto& model : models)
		{
			transparentObjects.push_back(createObject(model, pos, data, tag));
		}
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
		const auto cubeModel = MeshLoader().loadModel("Assets/Models/cube.obj");
		const auto suzaneModel = MeshLoader().loadModel("Assets/Models/suz.obj");
		
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				addModel((i + j) % 2 == 0?cubeModel: suzaneModel, { i * 3.5f-10, 0,j*3.5f-10 }, ShaderCollection::shaders["normals"], "modifiable");
			}
		}
		
		addModel(MeshLoader().loadModel("Assets/Models/sam2.obj"), {3.5f, 6, 0}, ShaderCollection::shaders["normals"], "modifiable");

		addTransparent(MeshLoader().loadModel("Assets/Models/cube.obj"), { 0, 4, -12 }, ShaderCollection::shaders["cubicCloud"]);

		createLightSourceBlock();
		
		//lights.push_back(std::make_shared<PointLight>(QVector3D{-5, 4, 7}));
		//lights.push_back(std::make_shared<PointLight>(QVector3D{30, 3, -7}, QColor{255, 23, 12}));
		
		lights.push_back(std::make_shared<SpotLight>(QVector3D{10, 2, 10}, QColor{200, 200, 200 }));

		backround = Background(funcions);
	}
	void addModel(const std::vector<MeshLoader::LoadedModel>& models, const QVector3D& pos, ShaderData& data, const std::string& tag = "")
	{
		for (const auto& model : models)
		{
			objects.push_back(createObject(model, pos, data, tag));
		}
	}
};
