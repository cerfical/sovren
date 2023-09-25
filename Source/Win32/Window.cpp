#include "Window.hpp"

#include "WinUiFactory.hpp"
#include "WinWindow.hpp"
#include "WinUtils.hpp"

#include <optional>
#include <Windowsx.h>

#define RENI_VKEY_MAPPING_LIST \
	RENI_VKEY_MAPPING(VK_LEFT, LeftArrow) \
	RENI_VKEY_MAPPING(VK_RIGHT, RightArrow) \
	RENI_VKEY_MAPPING(VK_UP, UpArrow) \
	RENI_VKEY_MAPPING(VK_DOWN, DownArrow)

namespace {
	using namespace RENI;

	std::optional<Keys> VKeyToKey(WPARAM vkey) noexcept {
#define RENI_VKEY_MAPPING(vkey, key) case vkey: { return Keys::key; }
		switch(vkey) {
			RENI_VKEY_MAPPING_LIST
			default: { return { }; }
		}
#undef RENI_VKEY_MAPPING
	}
}

#undef RENI_VKEY_MAPPING_LIST

namespace RENI {
	using namespace Win32;
		
	class Window::Impl : public WinWindow, public ImplBase {
	public:
		/** @{ */
		Impl(Window& window)
		 : m_window(window) {
			canvas = WinUiFactory::Get()->CreateCanvas(*this);
		}
		/** @} */

		std::unique_ptr<Canvas> canvas;

	private:
		/** @{ */
		LRESULT HandleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) override {
			switch(msg) {
				// mouse messages
				case WM_LBUTTONDOWN: { OnWmLButtonDown(); break; }
				case WM_LBUTTONUP: { OnWmLButtonUp(); break; }
				case WM_MBUTTONDOWN: { OnWmMButtonDown(); break; }
				case WM_MBUTTONUP: { OnWmMButtonUp(); break; }
				case WM_RBUTTONDOWN: { OnWmRButtonDown(); break; }
				case WM_RBUTTONUP: { OnWmRButtonUp(); break; }
				case WM_MOUSEMOVE: { OnWmMouseMove({ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) }); break; }
				// keyboard messages
				case WM_KEYDOWN: {
					const auto vkey = wParam;
					if(const auto key = VKeyToKey(vkey); key) {
						OnWmKeyDown(*key);
					}
					break;
				}
				case WM_KEYUP: {
					const auto vkey = wParam;
					if(const auto key = VKeyToKey(vkey); key) {
						OnWmKeyUp(*key);
					}
					break;
				}
				// window messages
				case WM_PAINT: { OnWmPaint(); break; }
				case WM_SIZE: { OnWmSize({ LOWORD(lParam), HIWORD(lParam) }); break; }
				case WM_SHOWWINDOW: { OnWmShowWindow(wParam == TRUE); break; }
				case WM_CLOSE: { OnWmClose(); break; }
				default: {
					return WinWindow::HandleMessage(hwnd, msg, wParam, lParam);
				}
			}
			return 0;
		}
		/** @} */


		/** @{ */
		/** @brief WM_LBUTTONDOWN message. */
		void OnWmLButtonDown() {
			m_window.m_mouse.PressButton(m_window, MouseButtons::Left);
		}

		/** @brief WM_LBUTTONUP message. */
		void OnWmLButtonUp() {
			m_window.m_mouse.ReleaseButton(m_window, MouseButtons::Left);
		}
		
		/** @brief WM_MBUTTONDOWN message. */
		void OnWmMButtonDown() {
			m_window.m_mouse.PressButton(m_window, MouseButtons::Middle);
		}

		/** @brief WM_MBUTTONUP message. */
		void OnWmMButtonUp() {
			m_window.m_mouse.ReleaseButton(m_window, MouseButtons::Middle);
		}
		
		/** @brief WM_RBUTTONDOWN message. */
		void OnWmRButtonDown() {
			m_window.m_mouse.PressButton(m_window, MouseButtons::Right);
		}
		
		/** @brief WM_RBUTTONUP message. */
		void OnWmRButtonUp() {
			m_window.m_mouse.ReleaseButton(m_window, MouseButtons::Right);
		}

		/** @brief WM_MOUSEMOVE message. */
		void OnWmMouseMove(const Point2D& cursorPos) {
			m_window.m_mouse.UpdateCursorPos(m_window, cursorPos);
		}
		/** @} */


		/** @{ */
		/** @brief WM_KEYDOWN message. */
		void OnWmKeyDown(Keys key) {
			m_window.m_keys.PressKey(m_window, key);
		}

		/** @brief WM_KEYUP message. */
		void OnWmKeyUp(Keys key) {
			m_window.m_keys.ReleaseKey(m_window, key);
		}
		/** @} */


		/** @{ */
		/** @brief WM_SIZE message. */
		void OnWmSize(const Extent2D& clientArea) {
			m_window.m_state.SetClientArea(m_window, clientArea);
		}

		/** @brief WM_PAINT message. */
		void OnWmPaint() {
			SafeWin32ApiCall(ValidateRect, GetHandle(), nullptr);
			m_window.OnDraw();
		}
		/** @} */


		/** @{ */
		/** @brief WM_SHOWWINDOW message. */
		void OnWmShowWindow(bool visible) {
			m_window.m_state.SetVisible(m_window, visible);
		}
		
		/** @brief WM_CLOSE message. */
		void OnWmClose() {
			SetVisible(false);
			m_window.OnClose();
		}
		/** @} */

		Window& m_window;
	};
}

namespace RENI {
	Window::Window() {
		m_impl.Init(std::make_unique<Impl>(*this));
		m_mouse.SetCursorPos(m_impl->GetCursorPos());
	}


	void Window::SetClientArea(const Extent2D& clientArea) {
		m_impl->SetClientArea(clientArea);
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
	

	void Window::SetMouseCapture() {
		m_impl->SetMouseCapture();
	}
	
	void Window::ReleaseMouseCapture() {
		m_impl->ReleaseMouseCapture();
	}


	Canvas& Window::GetCanvas() {
		return *m_impl->canvas;
	}
}