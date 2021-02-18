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
	QColorDialog* colorDialog;
	
	QBoxLayout* layout;
	GLWindow* viewport;

	bool initialized = false;
	
	AloyApplication(Window* window)
	{
		layout = new QBoxLayout(QBoxLayout::LeftToRight);
		viewport = new GLWindow(window);
		colorDialog = new QColorDialog();
		layout->setContentsMargins(0, 0, 0, 0);
		layout->addWidget(viewport);
		layout->addWidget(colorDialog);
		
		window->setLayout(layout);

	}
	
	void onUpdate() override
	{
		if (!initialized)//this has to ne
		{
			init();
			initialized = true;
		}

		render();
	}
	
	void init()
	{
		scene = Scene(viewport);
	 	viewport->glEnable(GL_DEPTH_TEST);

		colorDialog->setOption(QColorDialog::NoButtons);
	}

	void render()
	{
		const auto retinaScale = viewport->devicePixelRatio();
		viewport->glViewport(0, 0, viewport->width() * retinaScale, viewport->height() * retinaScale);

		viewport->glClearColor(0.08f, 0.08f, 0.08f, 1);
		viewport->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		scene.setColor(colorDialog->currentColor());

		scene.onUpdate();
		scene.onRender();
	}

};
