#pragma once
#include "OnUpdateSubscriber.h"
#include "Scene.h"
#include <QColorDialog>
#include <QBoxLayout>
#include "GLWindow.h"
#include "Window.h"
#include <QSlider>
#include "SceneRenderer.h"

class AloyApplication final: public OnUpdateSubscriber
{
public:
	Scene scene;
	QSlider* transformRatio;
	QSlider* subdivisionLevel;

	std::shared_ptr<QBoxLayout> layout;
	std::shared_ptr<GLWindow> viewport;

	bool initialized = false;
	SceneRenderer renderer;
	AloyApplication(Window* window)
	{
		layout = std::make_shared<QBoxLayout>(QBoxLayout::TopToBottom);
		viewport = std::make_shared<GLWindow>(window);
		
		QSurfaceFormat format;
		format.setSamples(16);
		format.setVersion(3, 3);
		format.setProfile(QSurfaceFormat::CoreProfile);

		viewport->setFormat(format);
		
		transformRatio = new QSlider(Qt::Horizontal);
		transformRatio->setRange(0, 100);
		transformRatio->setStyleSheet("QSlider::groove:horizontal {background-color:gray;}"
			"QSlider::handle:horizontal {background-color:white; height:16px; width: 16px;}");

		subdivisionLevel = new QSlider(Qt::Horizontal);
		subdivisionLevel->setRange(0, 2);
		subdivisionLevel->setStyleSheet("QSlider::groove:horizontal {background-color:gray;}"
			"QSlider::handle:horizontal {background-color:white; height:10px; width: 16px;}");
		
		layout->setContentsMargins(0, 0, 0, 0);
		layout->addWidget(viewport.get());
		layout->addWidget(transformRatio);
		layout->addWidget(subdivisionLevel);

		window->setLayout(layout.get());
		
	}

	void updateScene()
	{
		for(auto& obj: scene.objects)
		{
			if (obj->tag == "modifiable")
			{
				obj->renderer->shader->bind();
				obj->renderer->shader->setUniformValue("ratio", static_cast<float>(transformRatio->value()) / 100.0f);
				obj->renderer->shader->setUniformValue("subdivLevel", subdivisionLevel->value());
			}
		}
		
		scene.camera.aspectRatio = static_cast<float>(viewport->width()) / viewport->height();

		if (Input::keyJustPressed(Qt::Key_Z))
			renderer.nextDrawMode();

		scene.angularVelocity *= 0.987f;
		for (auto& object : scene.objects)
		{
			if (object->tag == "modifiable")
			{
				object->transform.rotate(QQuaternion::fromAxisAndAngle(scene.camera.right, -scene.angularVelocity.y()).normalized());
				object->transform.rotate(QQuaternion::fromAxisAndAngle(scene.camera.up, scene.angularVelocity.x()).normalized());
			}
		}
		if (Input::keyPressed(Qt::RightButton))
		{
			scene.angularVelocity += QVector2D(MouseInput::delta()) * 0.1f;
		}

		moveCamera();

		for (auto& light : scene.lights)
		{
			light->position = QQuaternion::fromAxisAndAngle({ 1,0,0 }, 0.2f) * light->position;
		}
	}

	void onUpdate() override
	{
		if (!initialized)//this has to ne
		{
			init();
			initialized = true;
		}
		
		updateScene();
		
		render();
	}
	
	void init()
	{
		scene = Scene(viewport);
	 	viewport->glEnable(GL_DEPTH_TEST);
	}

	void render()
	{
		const auto retinaScale = viewport->devicePixelRatio();
		viewport->glViewport(0, 0, viewport->width() * retinaScale, viewport->height() * retinaScale);

		viewport->glClearColor(0.08f, 0.08f, 0.08f, 1);
		viewport->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderer.render(scene);
	}

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
				scene.camera.translate(dir * 0.1f);
			}
		}
		if (Input::keyPressed(Qt::LeftButton))
			scene.camera.look(MouseInput::delta().x() * 0.5f, MouseInput::delta().y() * 0.5f);
	}
};
