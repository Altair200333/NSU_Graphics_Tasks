#pragma once
#include <memory>

#include <QWindow>

#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLPaintDevice>
#include <QPainter>

#include "Input.h"
#include "MouseInput.h"
#include <QKeyEvent>
#include <QOpenGLWidget>
#include <QBasicTimer>

class GLWindow : public QOpenGLWidget, public QOpenGLFunctions
{
Q_OBJECT
public:
	QBasicTimer timer;

	explicit GLWindow(QOpenGLWidget* parent = nullptr) : QOpenGLWidget{parent}
	{
		// This one inits OpenGL functions.
		//setSurfaceType(QWindow::OpenGLSurface);
	}

	virtual void init()
	{
	}

	virtual void render()
	{
		// Lazy init render device.
		if (!device_)
		{
			device_ = std::make_unique<QOpenGLPaintDevice>(size());
		}

		// Clear all buffers.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// Init sizes.
		const auto pixelRatio = devicePixelRatio();
		device_->setSize(size() * pixelRatio);
		device_->setDevicePixelRatio(pixelRatio);

		// Paint now.
		const QPainter painter{device_.get()};
		render(painter);
	}

	virtual void render(const QPainter& painter)
	{
	}

	//bunch of callbacks for input
	void keyPressEvent(QKeyEvent* e) override
	{
		Input::pressKey(e->nativeVirtualKey());
		QOpenGLWidget::keyPressEvent(e);
	}

	void keyReleaseEvent(QKeyEvent* e) override
	{
		Input::releaseKey(e->nativeVirtualKey());
		QOpenGLWidget::keyReleaseEvent(e);
	}

	void mouseMoveEvent(QMouseEvent* e) override
	{
		MouseInput::mouseCallback(e->pos());
		QOpenGLWidget::mouseMoveEvent(e);
	}

	void mousePressEvent(QMouseEvent* e) override
	{
		Input::pressKey(e->button());
		QOpenGLWidget::mousePressEvent(e);
	}

	void mouseReleaseEvent(QMouseEvent* e) override
	{
		Input::releaseKey(e->button());
		QOpenGLWidget::mouseReleaseEvent(e);
	}
	void initializeGL() override
	{
		initializeOpenGLFunctions();
		//makeCurrent();
		context = std::make_unique<QOpenGLContext>(this);
		context->create();
		
		init();
		timer.start(12, this);
	}
	void paintGL() override
	{
		render();
		
		MouseInput::reset();
		Input::reset();
	}
	void timerEvent(QTimerEvent* event) override
	{
		update();
	}
protected:
	virtual void onClose(){}
	bool event(QEvent* event) override
	{
		assert(event);
		switch (event->type())
		{
		case QEvent::UpdateRequest:
			update();
			return QOpenGLWidget::event(event);
		case QEvent::Close:
			onClose();
			return QOpenGLWidget::event(event);
		default:
			return QOpenGLWidget::event(event);
		}
	}

protected:
	std::shared_ptr<QOpenGLContext> context = nullptr;
	std::unique_ptr<QOpenGLPaintDevice> device_ = nullptr;
};
