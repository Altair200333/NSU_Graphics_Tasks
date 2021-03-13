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
#include <iomanip>
#include <sstream>
#include "FPSCounter.h"
#include "UIManager.h"

class AloyApplication final: public OnUpdateSubscriber
{
public:
	Scene scene;
	
	UIManager manager;

	bool initialized = false;
	SceneRenderer renderer;

	FPSCounter fpsCounter;
	float height = -5;
	AloyApplication(Window* window)
	{
		manager.setWindow(window);
		manager.init();
	}

	void updateFlashLight()
	{
		auto spot = std::dynamic_pointer_cast<SpotLight>(scene.lights.back());
		spot->position = scene.camera.position;
		spot->direction = scene.camera.front;
		if(Input::keyJustPressed(Qt::Key_F))
		{
			spot->intensity = spot->intensity > 0 ? 0 : 1;
		}
	}

	void updateScene()
	{
		scene.camera.aspectRatio = static_cast<float>(manager.viewport->width()) / manager.viewport->height();

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
			light->position = QQuaternion::fromAxisAndAngle({ 0,1,0 }, 20.0*fpsCounter.frameTime) * light->position;
		}
		
		updateFlashLight();
	}

	void updateFrameRate()
	{
		std::stringstream ss;
		ss << "framerate: " << std::fixed << std::setprecision(2) << 1.0f / fpsCounter.getFPS();
		manager.label->setText(QString(ss.str().c_str()));
	}

	void onUpdate() override
	{
		if (!initialized)
		{
			init();
			initialized = true;
		}

		updateFrameRate();
		
		updateScene();
		
		render();
	}
	
	void init()
	{
		scene = Scene(manager.viewport);
		manager.viewport->glEnable(GL_DEPTH_TEST);


		scene.addModel(MeshLoader().loadModel("Assets/Models/sam2.obj"), { 3.5f, 3, 0 }, ShaderCollection::shaders["normals"]);
		scene.addModel(MeshLoader().loadModel("Assets/Models/earth.obj"), { -3.5f, 3, 0 }, ShaderCollection::shaders["normals"], "modifiable");
		scene.addModel(MeshLoader().loadModel("Assets/Models/moon.obj"), { -3.5f, 3, 12 }, ShaderCollection::shaders["normals"], "modifiable");

		scene.addTransparent(MeshLoader().loadModel("Assets/Models/cube.obj"), { 0, 4, -12 }, ShaderCollection::shaders["cubicCloud"]);


		scene.addLight(std::make_shared<PointLight>(QVector3D{ -8, 4, 7 }, QColor{ 255, 255, 255 }, 2.5));
		scene.addLight(std::make_shared<PointLight>(QVector3D{ 30, 3, -7 }, QColor{ 255, 23, 12 }, 2));

		scene.addLight(std::make_shared<SpotLight>(QVector3D{ 10, 2, 10 }, QColor{ 200, 200, 200 }));
	}

	void render()
	{
		const auto retinaScale = manager.viewport->devicePixelRatio();
		manager.viewport->glViewport(0, 0, manager.viewport->width() * retinaScale, manager.viewport->height() * retinaScale);

		manager.viewport->glClearColor(0.08f, 0.08f, 0.08f, 1);
		manager.viewport->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
