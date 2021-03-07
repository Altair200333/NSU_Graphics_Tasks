#pragma once
#include <QSlider>
#include <QLabel>
#include "GLWindow.h"
#include <QBoxLayout>
#include <QColorDialog>
#include <QPushButton>
#include <QFont>

class UIManager final: public QObject
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

	QWidget* buttonLayoutGroup;
	Window* window;

	QColorDialog diffuseColorDialog;
	QColorDialog ambientColorDialog;
	QColorDialog specularColorDialog;
	
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
		horizontalBox = std::make_shared<QBoxLayout>(QBoxLayout::LeftToRight);
		
		button = new QPushButton("Diffuse");
		button1 = new QPushButton("Ambient");
		button2 = new QPushButton("Specular");
		
		connect(button, &QPushButton::released, this, &UIManager::showDiffuseColorDialog);
		connect(button1, &QPushButton::released, this, &UIManager::showAmbientColorDialog);
		connect(button2, &QPushButton::released, this, &UIManager::showSpecularColorDialog);

		horizontalBox->addWidget(button);
		horizontalBox->addWidget(button1);
		horizontalBox->addWidget(button2);
		buttonLayoutGroup = new QWidget();
		buttonLayoutGroup->setLayout(horizontalBox.get());
		//----
		buttonLayoutGroup->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

		verticalBox->addWidget(buttonLayoutGroup);
		
		window->setLayout(verticalBox.get());
	}
	void showDiffuseColorDialog()
	{
		diffuseColorDialog.show();
	}
	void showAmbientColorDialog()
	{
		ambientColorDialog.show();
	}
	void showSpecularColorDialog()
	{
		specularColorDialog.show();
	}
};