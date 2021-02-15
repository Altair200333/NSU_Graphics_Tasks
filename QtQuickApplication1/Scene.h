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

	Scene() = default;

	void createLightSourceBlock()
	{
		lightSourceBlock = std::make_shared<Object>();
		lightSourceBlock->mesh = Mesh(Cube::vertices, Cube::indices);
		lightSourceBlock->initRenderer(parent);
		lightSourceBlock->material.shadingMode = Material::materialColor;
		lightSourceBlock->transform.transform.scale(0.5f);
	}

	Scene(QObject* _parent): parent(_parent)
	{
		for (int i = 0; i < 11; ++i)
		{
			objects.push_back(std::make_shared<Object>());
			objects[i]->mesh = Mesh(Cube::vertices, Cube::indices);
			objects[i]->initRenderer(parent);
			objects[i]->transform.translate({ i * 3.5f - 5,0,-0.5});
		}
	
		createLightSourceBlock();
		
		lights.push_back(std::make_shared<LightSource>(QVector3D{0, 0, 7}));
		lights.push_back(std::make_shared<LightSource>(QVector3D{40, 9, -7}, QColor{200, 100, 10}));
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
	void onUpdate()
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
		if(Input::keyPressed(Qt::RightButton))
		{
			for (size_t i = 0; i < objects.size(); ++i)
			{
				objects[i]->transform.rotate(QQuaternion::fromAxisAndAngle(camera.up, MouseInput::delta().x()).normalized());
				objects[i]->transform.rotate(QQuaternion::fromAxisAndAngle(camera.right ,-MouseInput::delta().y()).normalized());
			}
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
			lightSourceBlock->renderer.render(camera, lights, true);
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
			glDisable(GL_CULL_FACE);
		}
		if (depthTest)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
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
