#pragma once
#include "OnUpdateSubscriber.h"
#include "Scene.h"
#include <QColorDialog>
#include <QBoxLayout>
#include "GLWindow.h"
#include "Window.h"
#include <QSlider>
#include <QLabel>
#include "SceneRenderer.h"
#include <chrono>
#include <ctime>

class AloyApplication final: public OnUpdateSubscriber
{
public:
	Scene scene;
	QLabel* label;
	QSlider* mixFactor;
	
	std::shared_ptr<QBoxLayout> layout;
	std::shared_ptr<GLWindow> viewport;

	bool initialized = false;
	SceneRenderer renderer;

	std::chrono::time_point<std::chrono::system_clock> lastFrameTime;
	
	AloyApplication(Window* window)
	{
		layout = std::make_shared<QBoxLayout>(QBoxLayout::TopToBottom);
		viewport = std::make_shared<GLWindow>(window);
		
		QSurfaceFormat format;
		format.setSamples(16);
		format.setVersion(3, 3);
		format.setProfile(QSurfaceFormat::CoreProfile);

		viewport->setFormat(format);

		//---
		label = new QLabel("framerate: xx");
		QFont f("Arial", 16, QFont::Bold);
		label->setFont(f);
		label->setStyleSheet("color: blue; background-color : white;");
		label->setMaximumHeight(20);
		//---
		mixFactor = new QSlider(Qt::Horizontal);
		mixFactor->setRange(0, 100);
		mixFactor->setSliderPosition(10);
		//---
		layout->setContentsMargins(0, 0, 0, 0);
		layout->addWidget(viewport.get());
		layout->addWidget(label);
		layout->addWidget(mixFactor);

		window->setLayout(layout.get());
		
	}

	void updateScene()
	{
		scene.camera.aspectRatio = static_cast<float>(viewport->width()) / viewport->height();

		if (Input::keyJustPressed(Qt::Key_Z))
			renderer.nextDrawMode();

		scene.angularVelocity *= 0.987f;
		
		for (auto& object : scene.objects)
		{
			object->material.roughness = static_cast<float>(mixFactor->value()) / 100.0f;
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
		if (!initialized)
		{
			init();
			initialized = true;
		}

		const auto current = std::chrono::system_clock::now();
		const std::chrono::duration<double> elapsedSeconds = current - lastFrameTime;
		lastFrameTime = current;
		const std::string text = "framerate: " + std::to_string(1.0f/ elapsedSeconds.count());
		label->setText(QString(text.c_str()));
		
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
