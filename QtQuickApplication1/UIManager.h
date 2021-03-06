#pragma once
#include <QSlider>
#include <QLabel>
#include "GLWindow.h"
#include <QBoxLayout>

class UIManager final
{
public:
	QLabel* label;
	QSlider* mixFactor;

	std::shared_ptr<QBoxLayout> layout;
	std::shared_ptr<GLWindow> viewport;
	
	Window* window;

	void setWindow(Window* _window)
	{
		window = _window;
	}
	UIManager() = default;

	void init()
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
		mixFactor->setSliderPosition(2);
		//---
		layout->setContentsMargins(0, 0, 0, 0);
		layout->addWidget(viewport.get());
		layout->addWidget(label);
		layout->addWidget(mixFactor);

		window->setLayout(layout.get());
	}
};