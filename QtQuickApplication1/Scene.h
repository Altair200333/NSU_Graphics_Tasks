#pragma once
#include <memory>
#include <vector>
#include <QObject>

#include "CubeData.h"
#include "Input.h"
#include "MouseInput.h"
#include "Object.h"

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
	GLCamera camera;

	bool cullFront = false;
	bool useVertexColor = false;
	
	Scene() = default;
	
	Scene(QObject* _parent): parent(_parent)
	{
		for (int i = 0; i < 11; ++i)
		{
			objects.push_back(std::make_shared<Object>());
			objects[i]->mesh = Mesh(Cube::vertices, Cube::indices);
			objects[i]->initRenderer(parent);
			objects[i]->transform.translate({ i * 3.5f - 5,0,0 });
		}
	}
	void setColor(const QColor& color)
	{
		for(auto& obj: objects)
		{
			obj->material.color = color;
		}
	}
	void setMode(Material::Mode mode)
	{
		for (auto& obj : objects)
		{
			obj->material.mode = mode;
		}
	}
	void onUpdate()
	{
		moveCamera();
		if(Input::keyJustPressed(Qt::Key_1))
		{
			if(useVertexColor)
				setMode(Material::materialColor);
			else
				setMode(Material::vertexColor);
			useVertexColor = !useVertexColor;
		}
		if (Input::keyJustPressed(Qt::Key_2))
		{
			cullFront = !cullFront;
		}
		
	}
	void onRender()
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
		for (size_t i = 0; i < objects.size(); ++i)
		{
			objects[i]->transform.rotate(1.0f - (i % 2) * 2, { 1,1,0 });
			objects[i]->renderer.render(camera);
		}
	}
};
