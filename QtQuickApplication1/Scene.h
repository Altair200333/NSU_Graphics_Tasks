#pragma once
#include <memory>
#include <utility>
#include <vector>
#include <QObject>

#include "Input.h"
#include "MouseInput.h"
#include "Object.h"
#include <QTransform>

#include "MeshLoader.h"

class Scene final
{
	std::shared_ptr<QObject> parent;
	
	void moveCamera()
	{
		std::map<int, QVector3D> controls = {
			{Qt::Key_W, {0,0,1}},
			{Qt::Key_S, {0,0,-1}},
			{Qt::Key_A, {-1,0,0}},
			{Qt::Key_D, {1,0,0}},
			{Qt::Key_E, {0,1,0}},
			{Qt::Key_Q, {0,-1,0}} };

		for (auto& [key, dir] : controls)
		{
			if (Input::keyPressed(key))
			{
				camera.translate(dir * 0.1f);
			}
		}
		if (Input::keyPressed(Qt::LeftButton))
			camera.look(MouseInput::delta().x() * 0.5f, MouseInput::delta().y() * 0.5f);
	}
	void createLightSourceBlock()
	{
		lightSourceBlock = std::make_shared<Object>();
		lightSourceBlock->mesh = MeshLoader().loadModel("Assets/Models/cube.obj")[0].mesh;
		lightSourceBlock->renderer = std::make_shared<SimpleMeshRenderer>();
		lightSourceBlock->initRenderer(parent);
		lightSourceBlock->material.shadingMode = Material::materialColor;
	}
	void addModel(const std::vector<MeshLoader::LoadedModel>& models, const QVector3D& pos, bool useGeometry = false, const std::string& tag = "")
	{
		for (const auto& model : models)
		{
			auto object = std::make_shared<Object>();
			object->tag = tag;
			object->mesh = model.mesh;
			object->material = model.material;
			object->renderer = std::make_shared<SimpleMeshRenderer>();
			
			if(useGeometry)
				object->initRenderer(parent, "Shaders/triangleG.fs", "Shaders/triangleG.vs", "Shaders/triangleG.gs");
			else
				object->initRenderer(parent, "Shaders/triangle.fs", "Shaders/triangle.vs");
			
			object->transform.translate(pos);
			objects.push_back(object);
		}

	}
public:
	std::vector<std::shared_ptr<Object>> objects;
	std::vector<std::shared_ptr<LightSource>> lights;

	std::shared_ptr<Object> lightSourceBlock;
	GLCamera camera;

	bool drawWireframe = false;

	QVector2D angularVelocity = {0,0};
	
	Scene() = default;

	Scene(std::shared_ptr<QObject> _parent): parent(std::move(_parent))
	{
		const auto cubeModel = MeshLoader().loadModel("Assets/Models/hcube.obj");
		const auto suzModel = MeshLoader().loadModel("Assets/Models/suz.obj");
		for (int i = 0; i < 2; ++i)
		{
			addModel(cubeModel, { i * 3.5f, 0,0 }, false, "modifiable");
		}
		for (int i = 0; i < 2; ++i)
		{
			addModel(suzModel, { 7.0f + i * 3.5f, 0,0 },false, "modifiable");
		}
		for (int i = 0; i < 2; ++i)
		{
			addModel(suzModel, { 7.0f + i * 3.5f, -5,0 }, true, "modifiable");
		}
		for (int i = 0; i < 2; ++i)
		{
			addModel(cubeModel, { i * 3.5f, -5,0 }, true, "modifiable");
		}
		
		addModel(MeshLoader().loadModel("Assets/Models/sam2.obj"), {3.5f, 5, 0}, false, "modifiable");
		addModel(MeshLoader().loadModel("Assets/Models/sam2.obj"), {7.5f, 5, 0}, false, "modifiable");
		addModel(MeshLoader().loadModel("Assets/Models/plane.obj"), {0, -8, 0}, false);

		createLightSourceBlock();
		
		lights.push_back(std::make_shared<LightSource>(QVector3D{-5, 0, 7}));
		lights.push_back(std::make_shared<LightSource>(QVector3D{30, 9, -7}, QColor{255, 219, 102}));
	}

	void onUpdate()
	{
		if (Input::keyJustPressed(Qt::Key_Z))
			drawWireframe = !drawWireframe;

		angularVelocity *= 0.987f;
		for (auto& object : objects)
		{
			if (object->tag == "modifiable")
			{
				object->transform.rotate(QQuaternion::fromAxisAndAngle(camera.right, -angularVelocity.y()).normalized());
				object->transform.rotate(QQuaternion::fromAxisAndAngle(camera.up, angularVelocity.x()).normalized());
			}
		}
		if(Input::keyPressed(Qt::RightButton))
		{
			angularVelocity += QVector2D(MouseInput::delta())*0.1f;
		}

		moveCamera();
		
		for (auto& light : lights)
		{
			light->position = QQuaternion::fromAxisAndAngle({1,0,0}, 0.2f) * light->position;
		}
	}

	void renderLights()
	{
		for(auto& light: lights)
		{
			lightSourceBlock->transform.translate(-lightSourceBlock->transform.position);
			lightSourceBlock->transform.translate(light->position);
			lightSourceBlock->material.shadingMode = Material::materialColor;
			lightSourceBlock->material.color = light->color;
			lightSourceBlock->material.isLightSource = true;
			lightSourceBlock->renderer->render(camera, lights);
		}
	}

	void onRender()
	{
		glPolygonMode(GL_FRONT_AND_BACK, drawWireframe ? GL_LINE : GL_FILL);

		for(size_t i = 0; i < objects.size(); ++i)
		{
			objects[i]->renderer->render(camera, lights);
		}
		
		renderLights();
	}
};
