#include "Window.hpp"
#include "WindowSystem.hpp"

namespace RENI {
	Window::Window() {
		m_handle.reset(WindowSystem::instance()->createWindow());

		m_clientSize = m_handle->clientSize();
		m_mouse.setCursorPos(m_handle->mousePos());

		m_handle->attachWindow(this);

		m_windowObservers.add(this);
		m_mouseObservers.add(this);
		m_keysObservers.add(this);
	}

	Window::~Window() {
		m_windowObservers.remove(this);
		m_mouseObservers.remove(this);
		m_keysObservers.remove(this);

		m_handle->detachWindow();
	}


	void Window::setClientSize(const Size2D& size) {
		if(m_clientSize != size) {
			handle()->setClientSize(size);
			triggerEvent(ResizeEvent(size, m_clientSize, this));
		}
	}

	void Window::triggerEvent(const Event& event) {
		onEvent(event);
	}

	void Window::onEvent(const Event& event) {
		if(const auto e = event.as<ResizeEvent>()) {
			if(m_clientSize != e->newSize()) {
				m_clientSize = e->newSize();
				m_windowObservers.trigger(&WindowObserver::onResize, *e);
			}
		} else if(const auto e = event.as<CloseEvent>()) {
			m_windowObservers.trigger(&WindowObserver::onClose, *e);
		} else if(const auto e = event.as<KeyPressEvent>()) {
			if(m_keys.isKeyReleased(e->pressedKey())) {
				m_keys.pressKey(e->pressedKey());
				m_keysObservers.trigger(&KeysObserver::onKeyPress, *e);
			}
		} else if(const auto e = event.as<KeyReleaseEvent>()) {
			if(m_keys.isKeyPressed(e->releasedKey())) {
				m_keys.releaseKey(e->releasedKey());
				m_keysObservers.trigger(&KeysObserver::onKeyRelease, *e);
			}
		} else if(const auto e = event.as<MousePressEvent>()) {
			if(m_mouse.isButtonReleased(e->pressedButton())) {
				m_mouse.pressButton(e->pressedButton());
				m_mouseObservers.trigger(&MouseObserver::onMousePress, *e);
			}
		} else if(const auto e = event.as<MouseReleaseEvent>()) {
			if(m_mouse.isButtonPressed(e->releasedButton())) {
				m_mouse.releaseButton(e->releasedButton());
				m_mouseObservers.trigger(&MouseObserver::onMouseRelease, *e);
			}
		} else if(const auto e = event.as<MouseMoveEvent>()) {
			if(m_mouse.cursorPos() != e->newPos()) {
				m_mouse.setCursorPos(e->newPos());
				m_mouseObservers.trigger(&MouseObserver::onMouseMove, *e);
			}
		}
	}
}