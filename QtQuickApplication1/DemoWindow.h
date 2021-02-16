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
		colorDialog.setOption(QColorDialog::NoButtons);
	}

	void render() override
	{
		const auto retinaScale = devicePixelRatio();
		glViewport(0, 0, width() * retinaScale, height() * retinaScale);
		
		glClearColor(0.08f, 0.08f, 0.08f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		
		scene.setColor(colorDialog.currentColor());
		
		scene.onUpdate();
		scene.onRender();

		MouseInput::reset();
		Input::reset();
	}
protected:
	void onClose() override
	{
		colorDialog.close();
	}
};
