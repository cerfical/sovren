#include "core/Window.hpp"
#include "pal.hpp"

#include <algorithm>
#include <vector>

namespace reni {
	struct Window::Impl : public pal::WindowCallbacks {
		void onWindowClose() override {
			palWindow->setVisible(false);
			visible = false;
		}


		void onWindowResize(Size2 newSize) override {
			if(newSize != clientSize) {
				window->onResize(newSize);
				clientSize = newSize;
			}
		}


		void onKeyStateChange(Keys k, bool pressed) override {
			const bool curKeyState = window->keyState(k);
			if(pressed) {
				if(!curKeyState) {
					window->onKeyDown(k);
					pressedKeys.push_back(k);
				}
			} else {
				if(curKeyState) {
					window->onKeyUp(k);
					std::erase(pressedKeys, k);
				}
			}
		}


		void onMouseButtonStateChange(MouseButtons b, bool pressed) override {
			const bool curButtonState = window->buttonState(b);
			if(pressed) {
				if(!curButtonState) {
					window->onButtonDown(b);
					pressedButtons.push_back(b);
				}

			} else {
				if(curButtonState) {
					window->onButtonUp(b);
					std::erase(pressedButtons, b);
				}
			}
		}


		void onMouseMove(Point2 newPos) override {
			if(newPos != mousePos) {
				window->onMouseMove(newPos);
				mousePos = newPos;
			}
		}


		std::unique_ptr<pal::Window> palWindow;
		Window* window = {};

		std::string title;
		Size2 clientSize;
		bool visible = false;
		
		std::vector<MouseButtons> pressedButtons;
		std::vector<Keys> pressedKeys;
		Point2 mousePos;
	};


	Window::Window() : m_impl(std::make_unique<Impl>()) {
		m_impl->palWindow = pal::Platform::get()->createWindow();
		m_impl->clientSize = m_impl->palWindow->getClientSize();
		m_impl->mousePos = m_impl->palWindow->getMousePos();

		m_impl->palWindow->installCallbacks(m_impl.get());
		m_impl->window = this;
	}


	void Window::setTitle(const std::string& newTitle) {
		m_impl->palWindow->setTitle(newTitle);
		m_impl->title = newTitle;
	}


	const std::string& Window::title() const {
		return m_impl->title;
	}


	void Window::setSize(Size2 newSize) {
		m_impl->palWindow->setClientSize(newSize);
	}


	Size2 Window::size() const {
		return m_impl->clientSize;
	}


	Point2 Window::mousePos() const {
		return m_impl->mousePos;
	}


	bool Window::keyState(Keys k) const {
		return std::ranges::find(m_impl->pressedKeys, k) != m_impl->pressedKeys.cend();
	}


	bool Window::buttonState(MouseButtons b) const {
		return std::ranges::find(m_impl->pressedButtons, b) != m_impl->pressedButtons.cend();
	}


	void* Window::nativeHandle() const {
		return m_impl->palWindow->nativeHandle();
	}


	void Window::show() {
		onShow();

		m_impl->palWindow->setVisible(true);
		m_impl->visible = true;

		const auto eventPoller = pal::Platform::get()->createEventPoller();
		while(m_impl->visible) {
			eventPoller->pollEvents();
			onUpdate();
		}

		onHide();
	}


	Window::Window(Window&&) noexcept = default;
	Window& Window::operator=(Window&&) noexcept = default;

	Window::~Window() = default;


	void Window::onResize(Size2) {}

	void Window::onShow() {}
	void Window::onUpdate() {}
	void Window::onHide() {}

	void Window::onKeyDown(Keys) {}
	void Window::onKeyUp(Keys) {}

	void Window::onButtonDown(MouseButtons) {}
	void Window::onButtonUp(MouseButtons) {}
	void Window::onMouseMove(Point2) {}
}