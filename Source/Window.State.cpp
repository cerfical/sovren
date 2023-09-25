#include "Window.hpp"

namespace RENI {
	void Window::State::SetClientArea(Window& window, const Extent2D& clientArea) {
		if(m_clientArea != clientArea) {
			m_clientArea = clientArea;
			window.GetCanvas().Resize(clientArea);
			window.OnResize();
		}
	}

	void Window::State::SetVisible(Window& window, bool visible) {
		if(m_visible != visible) {
			m_visible = visible;
			if(visible) {
				window.OnShow();
			} else {
				window.OnHide();
			}
		}
	}
}