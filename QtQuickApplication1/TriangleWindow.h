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

namespace
{
	struct Vertex
	{
		std::array<float, 3> vertex;
		std::array<float, 3> normal;
		std::array<float, 3> color;
	};
	std::vector<Vertex> points = {
		 //position           normal               color
		{{-1.0, -1.0,  1.0},   {0.0f,  1.0f, 0.0f},  {0.0f,  0.0f,  1.0f}},//0
		{{ 1.0, -1.0,  1.0},   {0.0f,  1.0f, 0.0f},  {0.0f,  0.0f,  1.0f}},//1
		{{ 1.0,  1.0,  1.0},   {0.0f,  1.0f, 0.0f},  {1.0f,  0.0f,  0.0f}},//2
		{{ -1.0,  1.0,  1.0},  {0.0f,  1.0f, 0.0f},  {0.0f,  0.0f,  1.0f}},//3
		{{-1.0, -1.0, -1.0,},  {0.0f,  0.0f, -1.0f},  {0.0f,  1.0f,  0.0f}},//4
		{{1.0, -1.0, -1.0},  {0.0f,  0.0f, -1.0f},  {1.0f,  0.0f,  1.0f}},//5
		{{1.0,  1.0, -1.0},  {0.0f,  0.0f,  1.0f},  {0.4f,  0.0f,  1.0f}},//6
		{{ -1.0,  1.0, -1.0},  {0.0f,  0.0f,  1.0f},  {0.0f,  0.0f,  1.0f}}//7
	};

	std::vector<Vertex> points2 = {
		//position           normal               color
	    {{-1.0, -1.0,  1.0},   {0.0f,  1.0f, 0.0f},  {0.0f,  0.0f,  1.0f}},//0
		{{ 1.0, -1.0,  1.0},   {0.0f,  1.0f, 0.0f},  {0.0f,  0.0f,  1.0f}},//1
		{{ 1.0,  1.0,  1.0},   {0.0f,  1.0f, 0.0f},  {1.0f,  0.0f,  0.0f}},//2
		{{ -1.0,  1.0,  1.0},  {0.0f,  1.0f, 0.0f},  {0.0f,  0.0f,  1.0f}},//3

		{{ 1.0, -1.0,  1.0},   {0.0f,  1.0f, 0.0f},  {0.0f,  0.0f,  1.0f}},//1
		{{1.0, -1.0, -1.0},  {0.0f,  0.0f, -1.0f},  {1.0f,  0.0f,  1.0f}},//5
		{{1.0,  1.0, -1.0},  {0.0f,  0.0f,  1.0f},  {0.4f,  0.0f,  1.0f}},//6
		{{ 1.0,  1.0,  1.0},   {0.0f,  1.0f, 0.0f},  {1.0f,  0.0f,  0.0f}},//2

		{{1.0, -1.0, -1.0},  {0.0f,  0.0f, -1.0f},  {1.0f,  0.0f,  1.0f}},//5
		{{1.0,  1.0, -1.0},  {0.0f,  0.0f,  1.0f},  {0.4f,  0.0f,  1.0f}},//6
		{{ -1.0,  1.0, -1.0},  {0.0f,  0.0f,  1.0f},  {0.0f,  0.0f,  1.0f}},//7
		{ {-1.0, -1.0, -1.0,},  {0.0f,  0.0f, -1.0f},  {0.0f,  1.0f,  0.0f}},//4

		{{-1.0, -1.0, -1.0,},  {0.0f,  0.0f, -1.0f},  {0.0f,  1.0f,  0.0f}},//4
		{{-1.0, -1.0,  1.0},   {0.0f,  1.0f, 0.0f},  {0.0f,  0.0f,  1.0f}},//0
		{{ -1.0,  1.0,  1.0},  {0.0f,  1.0f, 0.0f},  {0.0f,  0.0f,  1.0f}},//3
	};

	std::vector<unsigned int> indices = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3 };
}

namespace fgl
{
	
	class TriangleWindow final : public GLWindow
	{
		typedef void (*PglGenVertexArrays) (GLsizei n, GLuint* arrays);
		typedef void (*PglBindVertexArray) (GLuint array);
	public:
		GLCamera camera;
		QOpenGLBuffer m_vertexBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
		QOpenGLBuffer m_indexBuffer = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
		QOpenGLVertexArrayObject VAO = QOpenGLVertexArrayObject();
		QOpenGLExtraFunctions f;
		void init() override
		{
			f = QOpenGLExtraFunctions(context_.get());
			
			program_ = std::make_unique<QOpenGLShaderProgram>(this);
			program_->addShaderFromSourceFile(QOpenGLShader::Vertex,
				"Shaders/triangle.vs");
			program_->addShaderFromSourceFile(QOpenGLShader::Fragment,
				"Shaders/triangle.fs");

			auto s = program_->log();
			program_->link();
			
			VAO.create();
			VAO.bind();

			m_vertexBuffer.create();
			m_vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
			if (!m_vertexBuffer.bind())
			{
				qWarning() << "Could not bind vertex buffer to the context";
				return;
			}
			m_vertexBuffer.allocate(points.data(), points.size() * sizeof(Vertex));

			m_indexBuffer.create();
			m_indexBuffer.bind();
			m_indexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
			m_indexBuffer.allocate(indices.data(), indices.size() * sizeof(GLuint));
			
			program_->setAttributeBuffer("posAttr", GL_FLOAT, 0, 3, 9 * sizeof(float));
			program_->setAttributeBuffer("normalAttr", GL_FLOAT, 3 * sizeof(float), 3, 9 * sizeof(float));
			program_->setAttributeBuffer("colAttr", GL_FLOAT, 6 * sizeof(float), 3, 9 * sizeof(float));
			
			program_->enableAttributeArray("posAttr");
			program_->enableAttributeArray("colAttr");
			program_->enableAttributeArray("normalAttr");
			
			VAO.release();
		}

		void render() override
		{
			const auto retinaScale = devicePixelRatio();
			glViewport(0, 0, width() * retinaScale, height() * retinaScale);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);
			
			program_->bind();

			QMatrix4x4 model;
			model.rotate(100.0f * frame_ / screen()->refreshRate(), 0, 1, 0);
			 
			program_->setUniformValue(program_->uniformLocation("model"), model);
			program_->setUniformValue(program_->uniformLocation("view"), camera.getViewMatrix());
			program_->setUniformValue(program_->uniformLocation("projection"), camera.getProjectionMatrix());
			program_->setUniformValue(program_->uniformLocation("cameraPos"), camera.position);

			VAO.bind();
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
			VAO.release();

			program_->release();

			++frame_;
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
			camera.look(MouseInput::delta().x()*0.5f, MouseInput::delta().y() * 0.5f);
		}
	private:
		// Shader program handler.
		std::unique_ptr<QOpenGLShaderProgram> program_ = nullptr;
		
		// Frame counter for animation.
		int frame_ = 0;
	};
} // namespace fgl
