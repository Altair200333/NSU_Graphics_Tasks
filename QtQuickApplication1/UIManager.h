#pragma once
#include <QSlider>
#include <QLabel>
#include "GLWindow.h"
#include <QBoxLayout>
#include <QColorDialog>
#include <QPushButton>

class UIManager final
{
public:
	QLabel* label;
	QSlider* mixFactor;

	std::shared_ptr<QBoxLayout> verticalBox;
	std::shared_ptr<QBoxLayout> horizontalBox;
	std::shared_ptr<GLWindow> viewport;
	QPushButton* button;
	QPushButton* button1;
	QPushButton* button2;
	QWidget* child;
	Window* window;

	void setWindow(Window* _window)
	{
		window = _window;
	}
	UIManager() = default;

	void init()
	{
		verticalBox = std::make_shared<QBoxLayout>(QBoxLayout::TopToBottom);
		viewport = std::make_shared<GLWindow>(window);
		viewport->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);
		
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
		verticalBox->setContentsMargins(0, 0, 0, 0);
		verticalBox->addWidget(viewport.get());
		verticalBox->addWidget(label);
		verticalBox->addWidget(mixFactor);

		//----
		horizontalBox = std::make_shared<QBoxLayout>(QBoxLayout::RightToLeft);
		button = new QPushButton("PRESS ME");
		button1 = new QPushButton("PRESS ME 1");
		button2 = new QPushButton("PRESS ME 2");
		
		horizontalBox->addWidget(button);
		horizontalBox->addWidget(button1);
		horizontalBox->addWidget(button2);
		child = new QWidget();
		child->setLayout(horizontalBox.get());
		//----
		child->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

		verticalBox->addWidget(child);
		
		window->setLayout(verticalBox.get());
	}
};