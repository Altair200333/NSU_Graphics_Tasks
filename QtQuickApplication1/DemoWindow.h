#pragma once
#include "GLWindow.h"

#include <memory>

#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions>
#include <QScreen>
#include <QOpenGLExtraFunctions>
#include <array>
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

namespace fgl
{
	
	class DemoWindow final : public GLWindow
	{
	public:
		GLCamera camera;
		std::vector<std::shared_ptr<Object>> objects;
	
		void init() override
		{
			for(int i=0;i<4;++i)
			{
				objects.push_back(std::make_shared<Object>());
				objects[i]->mesh = Mesh(Cube::vertices, Cube::indices);
				objects[i]->initRenderer(this);
				objects[i]->transform.translate({ i*3.5f - 5,0,0 });
			}
		}

		void render() override
		{
			const auto retinaScale = devicePixelRatio();
			glViewport(0, 0, width() * retinaScale, height() * retinaScale);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);
			
			for (size_t i = 0; i < objects.size(); ++i)
			{
				objects[i]->transform.rotate(1.0f-(i%2)*2, { 1,1,0 });
				objects[i]->renderer.render(camera);
			}
			
			moveCamera();
			MouseInput::reset();
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
		void moveCamera()
		{
			std::map<int, QVector3D> controls = {
				{Qt::Key_W, {0,0,1}},
				{Qt::Key_S, {0,0,-1}},
				{Qt::Key_A, {-1,0,0}},
				{Qt::Key_D, {1,0,0}},
				{Qt::Key_E, {0,1,0}},
				{Qt::Key_Q, {0,-1,0}}};
			
			for(auto& [key, dir]: controls)
			{
				if (Input::keyPressed(key))
				{
					camera.translate(dir * 0.1f);
				}
			}
			if(Input::keyPressed(Qt::LeftButton))
				camera.look(MouseInput::delta().x()*0.5f, MouseInput::delta().y() * 0.5f);
		}
	private:		
		// Frame counter for animation.
		int frame_ = 0;
	};
} // namespace fgl
