#include "RenderWindow.hpp"

#include "pal.hpp"
#include "rhi.hpp"

namespace reni {
	struct RenderWindow::Impl : public pal::WindowCallbacks {
		void onWindowClose() override {
			palWindow->setVisible(false);
			visible = false;

			renderWindow->onClose();
		}

		void onWindowResize(Size2D newSize) override {
			if(newSize != clientSize) {
				renderWindow->onResize(std::exchange(clientSize, newSize), newSize);
			}
		}

		void onKeyStateChange(Keys key, bool pressed) override {
			if(pressed) {
				renderWindow->onKeyDown(key);
			} else {
				renderWindow->onKeyUp(key);
			}
		}

		void onMouseButtonStateChange(MouseButtons button, bool pressed) override {
			if(pressed) {
				renderWindow->onButtonDown(button);
			} else {
				renderWindow->onButtonUp(button);
			}
		}

		void onMouseMove(Point2D newPos) override {
			if(newPos != mousePos) {
				renderWindow->onMouseMove(std::exchange(mousePos, newPos), newPos);
			}
		}

		std::unique_ptr<pal::Window> palWindow;
		RenderWindow* renderWindow = {};

		std::unique_ptr<rhi::SwapChain> swapChain;

		std::string title;
		Size2D clientSize;
		Point2D mousePos;
		Color fillColor;

		bool visible = false;
	};


	RenderWindow::RenderWindow()
		: m_impl(std::make_unique<Impl>()) {

		auto platform = pal::Platform::get();
		m_impl->palWindow = platform->createWindow();

		auto renderApi = platform->createRenderBackend();
		m_impl->swapChain = renderApi->createSwapChain(m_impl->palWindow->nativeHandle());

		m_impl->clientSize = m_impl->palWindow->getClientSize();
		m_impl->mousePos = m_impl->palWindow->getMousePos();

		m_impl->palWindow->installCallbacks(m_impl.get());
		m_impl->renderWindow = this;
	}


	void RenderWindow::show() {
		m_impl->palWindow->setVisible(true);
		m_impl->visible = true;

		auto eventPoller = pal::Platform::get()->createEventPoller();
		while(m_impl->visible) {
			eventPoller->pollEvents();
			m_impl->swapChain->swapBuffers();
		}
	}


	void RenderWindow::setTitle(const std::string& newTitle) {
		m_impl->palWindow->setTitle(newTitle);
		m_impl->title = newTitle;
	}


	const std::string& RenderWindow::title() const {
		return m_impl->title;
	}


	void RenderWindow::setSize(Size2D newSize) {
		m_impl->palWindow->setClientSize(newSize);
	}


	Size2D RenderWindow::size() const {
		return m_impl->clientSize;
	}


	Point2D RenderWindow::mousePos() const {
		return m_impl->mousePos;
	}


	void RenderWindow::setFillColor(Color fillColor) {
		m_impl->fillColor = fillColor;
	}


	Color RenderWindow::fillColor() const {
		return m_impl->fillColor;
	}


	RenderWindow::RenderWindow(RenderWindow&&) noexcept = default;
	RenderWindow& RenderWindow::operator=(RenderWindow&&) noexcept = default;

	RenderWindow::~RenderWindow() = default;
}