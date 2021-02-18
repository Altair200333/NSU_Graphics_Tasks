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
#include <QMainWindow>
#include <QColorDialog>
#include <QTimer>
class GLWindow : public QOpenGLWidget, public QOpenGLFunctions
{
Q_OBJECT
public:
	QTimer* timer;
	virtual void init(){}
	virtual void render(){}

	GLWindow()
	{
		timer = new QTimer(this);
		connect(timer, SIGNAL(timeout()), this, SLOT(update()));
		timer->start(10);
		setMouseTracking(true);
	}
	void initializeGL() override
	{
		initializeOpenGLFunctions();
		context = std::make_unique<QOpenGLContext>(this);

		init();
	}
	void resizeGL(int w, int h) override
	{
		
	}
	void paintGL() override
	{
		render();
		Input::reset();
		MouseInput::reset();
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


protected:
	virtual void onClose() {}
	bool event(QEvent* event) override
	{
		assert(event);
		switch (event->type())
		{
		case QEvent::Close:
			onClose();
			return QOpenGLWidget::event(event);
		default:
			return QOpenGLWidget::event(event);
		}
	}

protected:
	std::shared_ptr<QOpenGLContext> context = nullptr;
};
