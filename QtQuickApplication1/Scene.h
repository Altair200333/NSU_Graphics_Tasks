#pragma once
#include <memory>
#include <vector>
#include <QObject>

#include "CubeData.h"
#include "Input.h"
#include "MouseInput.h"
#include "Object.h"
#include <QTransform>

#include "CubeGenerator.h"
#include "MeshLoader.h"

class Scene final
{
	QObject* parent;

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
public:
	std::vector<std::shared_ptr<Object>> objects;
	std::vector<std::shared_ptr<LightSource>> lights;

	std::shared_ptr<Object> lightSourceBlock;
	GLCamera camera;

	bool cullFront = false;
	bool depthTest = true;
	bool useVertexColor = false;
	bool useMultiSapling = true;
	bool drawWireframe = false;

	QVector2D torgue = {0,0};
	
	Scene() = default;

	void createLightSourceBlock()
	{
		lightSourceBlock = std::make_shared<Object>();
		lightSourceBlock->mesh = Mesh(Cube::vertices, Cube::indices);
		lightSourceBlock->initRenderer(parent);
		lightSourceBlock->material.shadingMode = Material::materialColor;
		lightSourceBlock->transform.transform.scale(0.5f);
	}
	void addModel(const std::vector<MeshLoader::LoadedModel>& models, const QVector3D& pos)
	{
		for (size_t i = 0; i<models.size(); ++i)
		{
			auto object = std::make_shared<Object>();
			object->mesh = models[i].mesh;
			object->material = models[i].material;
			object->initRenderer(parent);
			object->transform.translate(pos);
			objects.push_back(object);
		}
		
	}
	Scene(QObject* _parent): parent(_parent)
	{
		auto cubeModel = MeshLoader().loadModel("Assets/Models/cube.obj");
		auto suzModel = MeshLoader().loadModel("Assets/Models/suz.obj");
		for (int i = 0; i < 4; ++i)
		{
			addModel(cubeModel, { i * 3.5f, 0,0 });
		}
		for (int i = 0; i < 4; ++i)
		{
			addModel(suzModel, { 17.0f + i * 3.5f, 0,0 });
		}
		
		addModel(MeshLoader().loadModel("Assets/Models/cont.obj"), { 5, 7,-4 });

		createLightSourceBlock();
		
		lights.push_back(std::make_shared<LightSource>(QVector3D{0, 0, 7}));
		lights.push_back(std::make_shared<LightSource>(QVector3D{50, 9, -7}, QColor{20, 20, 200}));
	}
	void setColor(const QColor& color)
	{
		for(auto& obj: objects)
		{
			obj->material.color = color;
		}
	}
	void setShadingMode(Material::ShadingMode mode)
	{
		for (auto& obj : objects)
		{
			obj->material.shadingMode = mode;
		}
	}

	void switchParams()
	{
		if(Input::keyJustPressed(Qt::Key_1))
		{
			if(useVertexColor)
				setShadingMode(Material::materialColor);
			else
				setShadingMode(Material::vertexColor);
			useVertexColor = !useVertexColor;
		}
		if (Input::keyJustPressed(Qt::Key_2))
		{
			cullFront = !cullFront;
		}
		if (Input::keyJustPressed(Qt::Key_3))
		{
			depthTest = !depthTest;
		}
		if (Input::keyJustPressed(Qt::Key_4))
		{
			useMultiSapling = !useMultiSapling;
		}
		if (Input::keyJustPressed(Qt::Key_Z))
		{
			drawWireframe = !drawWireframe;
		}
	}

	void onUpdate()
	{
		switchParams();

		torgue *= 0.98f;
		for (auto& object : objects)
		{
			object->transform.rotate(QQuaternion::fromAxisAndAngle(camera.right, -torgue.y()).normalized());
			object->transform.rotate(QQuaternion::fromAxisAndAngle(camera.up, torgue.x()).normalized());
		}
		if(Input::keyPressed(Qt::RightButton))
		{
			torgue += QVector2D(MouseInput::delta())*0.1f;
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
			lightSourceBlock->renderer.render(camera, lights);
		}
	}

	void enableParams()
	{
		if (cullFront)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
		}
		else
		{
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);

		}
		if (depthTest)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
		if (useMultiSapling)
			glEnable(GL_MULTISAMPLE);
		else
			glDisable(GL_MULTISAMPLE);

		glPolygonMode(GL_FRONT_AND_BACK, drawWireframe ? GL_LINE : GL_FILL);
	}

	void onRender()
	{
		enableParams();
		
		for(size_t i = 0; i < objects.size(); ++i)
		{
			objects[i]->renderer.render(camera, lights);
		}
		
		renderLights();
	}
};
