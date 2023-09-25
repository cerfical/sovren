#include "Window.hpp"

#include "WinWindow.hpp"
#include "EventLoop.hpp"

namespace RENI {
	struct Window::Impl : public WinWindow {
		/** @{ */
		explicit Impl(Window& window)
			: window(window) {
			clientSize = GetClientSize();
			mouse.SetCursorPos(GetCursorPos());
		}
		/** @} */

		/** @{ */
		void OnResize(const Size2D& newSize) override {
			if(clientSize != newSize) {
				const auto oldSize = std::exchange(clientSize, newSize);
				window.OnResize(newSize, oldSize);
			}
		}
		
		void OnClose() override {
			window.OnClose();
		}
		/** @} */

		/** @{ */
		void OnKeyPress(Keys pressedKey) override {
			keys.PressKey(pressedKey);
			window.OnKeyPress(pressedKey);
		}

		void OnKeyRelease(Keys releasedKey) override {
			keys.ReleaseKey(releasedKey);
			window.OnKeyRelease(releasedKey);
		}
		/** @} */

		/** @{ */
		void OnMousePress(MouseButtons pressedButton) override {
			mouse.PressButton(pressedButton);
			window.OnMousePress(pressedButton);
		}

		void OnMouseRelease(MouseButtons releasedButton) override {
			mouse.ReleaseButton(releasedButton);
			window.OnMouseRelease(releasedButton);
		}

		void OnMouseMove(const Point2D& newPos) override {
			if(mouse.GetCursorPos() != newPos) {
				const auto oldPos = mouse.GetCursorPos();
				mouse.SetCursorPos(newPos);
				window.OnMouseMove(newPos, oldPos);
			}
		}
		/** @} */

		Size2D clientSize;
		MouseState mouse;
		KeysState keys;

		Window& window;
	};
}

namespace RENI {
	void Window::OnClose() {
		SetVisible(false);
		EventLoop::Quit(0);
	}


	Window::Window()
		: m_impl(std::make_unique<Impl>(*this))
	{ }

	Window::~Window() = default;


	void Window::Resize(const Size2D& size) {
		if(m_impl->clientSize != size) {
			m_impl->SetClientSize(size);
		}
	}

	const Size2D& Window::GetSize() const {
		return m_impl->clientSize;
	}


	void Window::SetTitle(std::string_view title) {
		m_impl->SetTitle(title);
	}

	std::string Window::GetTitle() const {
		return m_impl->GetTitle();
	}


	void Window::SetVisible(bool visible) {
		m_impl->SetVisible(visible);
	}

	bool Window::IsVisible() const {
		return m_impl->IsVisible();
	}


	const KeysState& Window::GetKeysState() const noexcept {
		return m_impl->keys;
	}

	const MouseState& Window::GetMouseState() const noexcept {
		return m_impl->mouse;
	}


	void Window::ToggleMouseCapture() {
		m_impl->ToggleMouseCapture();
	}
}