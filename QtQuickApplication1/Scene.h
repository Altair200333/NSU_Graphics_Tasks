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

	void onUpdate()
	{
		moveCamera();
	}
	void onRender()
	{
		for (size_t i = 0; i < objects.size(); ++i)
		{
			objects[i]->transform.rotate(1.0f - (i % 2) * 2, { 1,1,0 });
			objects[i]->renderer.render(camera);
		}
	}
};
