#pragma once
#include <memory>

#include <QWindow>

#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLPaintDevice>
#include <QPainter>

class QEvent;
class QExposeEvent;

namespace fgl {

	class GLWindow : public QWindow, public QOpenGLFunctions
	{
	Q_OBJECT
	public:
		explicit GLWindow(QWindow* parent = nullptr) : QWindow{ parent }
		{
			// This one inits OpenGL functions.
			setSurfaceType(QWindow::OpenGLSurface);
		}

		virtual void init(){}

		virtual void render()
		{
			// Lazy init render device.
			if (!device_) {
				device_ = std::make_unique<QOpenGLPaintDevice>(size());
			}

			// Clear all buffers.
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			// Init sizes.
			const auto pixelRatio = devicePixelRatio();
			device_->setSize(size() * pixelRatio);
			device_->setDevicePixelRatio(pixelRatio);

			// Paint now.
			const QPainter painter{ device_.get() };
			render(painter);
		}
		
		virtual void render(const QPainter& painter) {}

	public:
		void setAnimated(bool animating = false)
		{
			animating_ = animating;
		}

	public slots:
		void renderNow()
		{
			// If not exposed yet then skip render.
			if (!isExposed()) {
				return;
			}

			auto needsInitialize = false;

			// Lazy init gl context.
			if (!context) {
				context = std::make_unique<QOpenGLContext>(this);
				context->setFormat(requestedFormat());
				context->create();

				needsInitialize = true;
			}

			const bool contextBindSuccess = context->makeCurrent(this);
			if (!contextBindSuccess) {
				return;
			}

			if (needsInitialize) {
				initializeOpenGLFunctions();
				init();
			}

			// Render now then swap buffers.
			render();

			context->swapBuffers(this);

			// Post message to redraw later if animating.
			if (animating_) {
				renderLater();
			}
		}
		void renderLater()
		{
			//Post message to request window surface redraw.
			requestUpdate();
		}

	protected:
		bool event(QEvent* event) override
		{
			assert(event);
			switch (event->type()) {
			case QEvent::UpdateRequest:
				// In case someone requested update we render inplace.
				renderNow();
				return true;
			default:
				return QWindow::event(event);
			}
		}

		void exposeEvent(QExposeEvent* event) override
		{
			if (isExposed()) {
				renderNow();
			}
		}

	protected:
		bool animating_ = false;
		std::shared_ptr<QOpenGLContext> context = nullptr;
		std::unique_ptr<QOpenGLPaintDevice> device_ = nullptr;
	};

} // namespace fgl
