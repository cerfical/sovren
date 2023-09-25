#include "Window.hpp"

#include "WinUiFactory.hpp"
#include "WinWindow.hpp"

#include <optional>
#include <Windowsx.h>

#define RENI_VKEY_MAPPING_LIST \
	RENI_VKEY_MAPPING(VK_LEFT, LeftArrow) \
	RENI_VKEY_MAPPING(VK_RIGHT, RightArrow) \
	RENI_VKEY_MAPPING(VK_UP, UpArrow) \
	RENI_VKEY_MAPPING(VK_DOWN, DownArrow)

namespace RENI {
	class Window::Impl : public WinWindow, public ImplBase {
	public:
		/** @{ */
		Impl(Window& window)
			: window(window) {
			canvas = WinUiFactory::Get()->CreateCanvas(*this);
		}
		/** @} */

		std::unique_ptr<Canvas> canvas;
		Window& window;

	private:
		/** @{ */
		static std::optional<Keys> VKeyToKey(WPARAM vkey) noexcept {
#define RENI_VKEY_MAPPING(vkey, key) case vkey: { return Keys::key; }
			switch(vkey) {
				RENI_VKEY_MAPPING_LIST
			}
#undef RENI_VKEY_MAPPING
			return { };
		}
		/** @} */


		/** @{ */
		LRESULT OnMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) override {
			switch(msg) {
				// mouse messages
				case WM_LBUTTONDOWN: { OnLButtonDown(); break; }
				case WM_LBUTTONUP: { OnLButtonUp(); break; }
				case WM_MBUTTONDOWN: { OnMButtonDown(); break; }
				case WM_MBUTTONUP: { OnMButtonUp(); break; }
				case WM_RBUTTONDOWN: { OnRButtonDown(); break; }
				case WM_RBUTTONUP: { OnRButtonUp(); break; }
				case WM_MOUSEMOVE: { OnMouseMove({ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) }); break; }
				// keyboard messages
				case WM_KEYDOWN: {
					if(const auto key = VKeyToKey(wParam); key) {
						OnKeyDown(*key);
					}
					break;
				}
				case WM_KEYUP: {
					if(const auto key = VKeyToKey(wParam); key) {
						OnKeyUp(*key);
					}
					break;
				}
				// window messages
				case WM_PAINT: { OnPaint(); break; }
				case WM_SIZE: { OnSize({ LOWORD(lParam), HIWORD(lParam) }); break; }
				case WM_SHOWWINDOW: { OnShowWindow(wParam == TRUE); break; }
				case WM_CLOSE: { OnClose(); break; }
				default: {
					return WinWindow::OnMessage(hwnd, msg, wParam, lParam);
				}
			}
			return 0;
		}
		/** @} */


		/** @{ */
		/** @brief WM_LBUTTONDOWN message. */
		void OnLButtonDown() {
			window.mouse.PressButton(window, MouseButtons::Left);
		}

		/** @brief WM_LBUTTONUP message. */
		void OnLButtonUp() {
			window.mouse.ReleaseButton(window, MouseButtons::Left);
		}
		
		/** @brief WM_MBUTTONDOWN message. */
		void OnMButtonDown() {
			window.mouse.PressButton(window, MouseButtons::Middle);
		}

		/** @brief WM_MBUTTONUP message. */
		void OnMButtonUp() {
			window.mouse.ReleaseButton(window, MouseButtons::Middle);
		}
		
		/** @brief WM_RBUTTONDOWN message. */
		void OnRButtonDown() {
			window.mouse.PressButton(window, MouseButtons::Right);
		}
		
		/** @brief WM_RBUTTONUP message. */
		void OnRButtonUp() {
			window.mouse.ReleaseButton(window, MouseButtons::Right);
		}

		/** @brief WM_MOUSEMOVE message. */
		void OnMouseMove(const Point2D& cursorPos) {
			window.mouse.UpdateCursorPos(window, cursorPos);
		}
		/** @} */


		/** @{ */
		/** @brief WM_KEYDOWN message. */
		void OnKeyDown(Keys key) {
			window.keys.PressKey(window, key);
		}

		/** @brief WM_KEYUP message. */
		void OnKeyUp(Keys key) {
			window.keys.ReleaseKey(window, key);
		}
		/** @} */


		/** @{ */
		/** @brief WM_SIZE message. */
		void OnSize(const Extent2D& clientArea) {
			window.UpdateClientArea(clientArea);
		}

		/** @brief WM_PAINT message. */
		void OnPaint() {
			window.OnDraw();
			SafeWndProcCall(ValidateRect, GetHandle(), nullptr);
		}
		/** @} */


		/** @{ */
		/** @brief WM_SHOWWINDOW message. */
		void OnShowWindow(bool visible) {
			window.UpdateVisibility(visible);
		}
		
		/** @brief WM_CLOSE message. */
		void OnClose() {
			SetVisible(false);
			window.OnClose();
		}
		/** @} */
	};


	Window::Window() {
		m_impl.Init(std::make_unique<Impl>(*this));
		m_clientArea = m_impl->GetClientArea();
		mouse.SetCursorPos(m_impl->GetCursorPos());
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
	

	void Window::CaptureMouse() {
		m_impl->CaptureMouse();
	}
	
	void Window::ReleaseMouse() {
		m_impl->ReleaseMouse();
	}


	Canvas* Window::GetCanvas() {
		return m_impl->canvas.get();
	}
}