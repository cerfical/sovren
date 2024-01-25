#ifndef RENI_WINDOW_IMPL_BASE_HEADER
#define RENI_WINDOW_IMPL_BASE_HEADER

#include "Window.hpp"

#include <algorithm>
#include <vector>

namespace RENI {
	
	struct Window::ImplBase {

		explicit ImplBase(Window& window)
			: window(window)
		{ }


		void pressButton(MouseButtons b) {
			if(!buttonState(b)) {
				m_pressedButtons.push_back(b);
				window.onMousePress(b);
			}
		}

		void releaseButton(MouseButtons b) {
			if(buttonState(b)) {
				std::erase(m_pressedButtons, b);
				window.onMouseRelease(b);
			}
		}

		bool buttonState(MouseButtons b) const {
			return std::ranges::find(m_pressedButtons, b) != m_pressedButtons.cend();
		}


		void pressKey(Keys k) {
			if(!keyState(k)) {
				m_pressedKeys.push_back(k);
				window.onKeyPress(k);
			}
		}

		void releaseKey(Keys k) {
			if(keyState(k)) {
				std::erase(m_pressedKeys, k);
				window.onKeyRelease(k);
			}
		}

		bool keyState(Keys k) const {
			return std::ranges::find(m_pressedKeys, k) != m_pressedKeys.cend();
		}


		void updateCursor(Point2D newPos) {
			if(cursorPos != newPos) {
				const auto oldPos = std::exchange(cursorPos, newPos);
				window.onMouseMove(newPos, oldPos);
			}
		}

		void updateSize(Size2D newSize) {
			if(clientSize != newSize) {
				const auto oldSize = std::exchange(clientSize, newSize);
				window.onResize(newSize, oldSize);
			}
		}


		Size2D clientSize;
		Point2D cursorPos;

		Window& window;


	private:
		std::vector<MouseButtons> m_pressedButtons;
		std::vector<Keys> m_pressedKeys;
	};

}

#endif