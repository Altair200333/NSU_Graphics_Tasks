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

class GLWindow : public QWindow, public QOpenGLFunctions
{
Q_OBJECT
public:
	explicit GLWindow(QWindow* parent = nullptr) : QWindow{parent}
	{
		// This one inits OpenGL functions.
		setSurfaceType(QWindow::OpenGLSurface);
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
		QWindow::keyPressEvent(e);
	}

	void keyReleaseEvent(QKeyEvent* e) override
	{
		Input::releaseKey(e->nativeVirtualKey());
		QWindow::keyReleaseEvent(e);
	}

	void mouseMoveEvent(QMouseEvent* e) override
	{
		MouseInput::mouseCallback(e->pos());
		QWindow::mouseMoveEvent(e);
	}

	void mousePressEvent(QMouseEvent* e) override
	{
		Input::pressKey(e->button());
		QWindow::mousePressEvent(e);
	}

	void mouseReleaseEvent(QMouseEvent* e) override
	{
		Input::releaseKey(e->button());
		QWindow::mouseReleaseEvent(e);
	}

public slots:
	void renderNow()
	{
		// If not exposed yet then skip render.
		if (!isExposed())
		{
			return;
		}

		auto needsInitialize = false;

		// Lazy init gl context.
		if (!context)
		{
			context = std::make_unique<QOpenGLContext>(this);
			context->setFormat(requestedFormat());
			context->create();

			needsInitialize = true;
		}

		const bool contextBindSuccess = context->makeCurrent(this);
		if (!contextBindSuccess)
		{
			return;
		}

		if (needsInitialize)
		{
			initializeOpenGLFunctions();
			init();
		}

		// Render now then swap buffers.
		render();

		context->swapBuffers(this);

		requestUpdate();
	}

protected:
	virtual void onClose(){}
	bool event(QEvent* event) override
	{
		assert(event);
		switch (event->type())
		{
		case QEvent::UpdateRequest:
			// In case someone requested update we render inplace.
			renderNow();
			return true;
		case QEvent::Close:
			onClose();
			return QWindow::event(event);
		default:
			return QWindow::event(event);
		}
	}

	void exposeEvent(QExposeEvent* event) override
	{
		if (isExposed())
		{
			renderNow();
		}
	}

protected:
	std::shared_ptr<QOpenGLContext> context = nullptr;
	std::unique_ptr<QOpenGLPaintDevice> device_ = nullptr;
};
