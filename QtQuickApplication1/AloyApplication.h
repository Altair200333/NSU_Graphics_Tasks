#pragma once
#include "OnUpdateSubscriber.h"
#include "Scene.h"
#include <QColorDialog>
#include <QBoxLayout>
#include "GLWindow.h"
#include "Window.h"

class AloyApplication final: public OnUpdateSubscriber
{
public:
	Scene scene;
	
	std::shared_ptr<QBoxLayout> layout;
	std::shared_ptr<GLWindow> viewport;

	bool initialized = false;
	
	AloyApplication(Window* window)
	{
		layout = std::make_shared<QBoxLayout>(QBoxLayout::TopToBottom);
		viewport = std::make_shared<GLWindow>(window);

		layout->setContentsMargins(0, 0, 0, 0);
		layout->addWidget(viewport.get());
		
		window->setLayout(layout.get());
		
	}
	
	void onUpdate() override
	{
		if (!initialized)//this has to ne
		{
			init();
			initialized = true;
		}

		scene.camera.aspectRatio = static_cast<float>(viewport->width()) / viewport->height();
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

		scene.onUpdate();
		scene.onRender();
	}

};
