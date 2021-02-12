#pragma once
#include "GLWindow.h"

#include <memory>

#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions>
#include <QScreen>
#include <QOpenGLExtraFunctions>
#include <array>
#include <QColorDialog>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>


#include "GLCamera.h"
#include <QWindow>
#include "Input.h"
#include "MouseInput.h"
#include "CubeData.h"
#include "Vertex.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "Object.h"
#include "Scene.h"

class DemoWindow final : public GLWindow
{
public:
	Scene scene;
	QColorDialog colorDialog;

	void init() override
	{
		scene = Scene(this);

		colorDialog.show();
	}

	void render() override
	{
		const auto retinaScale = devicePixelRatio();
		glViewport(0, 0, width() * retinaScale, height() * retinaScale);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		scene.setColor(colorDialog.currentColor());
		
		scene.onUpdate();
		scene.onRender();

		MouseInput::reset();
		Input::reset();
	}

	void keyPressEvent(QKeyEvent* e) override
	{
		Input::pressKey(e->key());
		GLWindow::keyPressEvent(e);
	}

	void keyReleaseEvent(QKeyEvent* e) override
	{
		Input::releaseKey(e->key());
		GLWindow::keyReleaseEvent(e);
	}

	void mouseMoveEvent(QMouseEvent* e) override
	{
		MouseInput::mouseCallback(e->pos());
	}

	void mousePressEvent(QMouseEvent* e) override
	{
		Input::pressKey(e->button());
		GLWindow::mousePressEvent(e);
	}

	void mouseReleaseEvent(QMouseEvent* e) override
	{
		Input::releaseKey(e->button());
		GLWindow::mouseReleaseEvent(e);
	}
};
