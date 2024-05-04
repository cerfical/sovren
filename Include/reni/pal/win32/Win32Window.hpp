#ifndef RENI_PAL_WIN32_WINDOW_HEADER
#define RENI_PAL_WIN32_WINDOW_HEADER

#include "../Window.hpp"
#include "../WindowCallbacks.hpp"

#include "WndClass.hpp"
#include "util.hpp"

#include <optional>

#include <Windows.h>
#include <windowsx.h>

namespace reni::pal::win32 {

	class Win32Window : public Window {
	public:

		Win32Window() {
			// initialize a new Win32 window class
			static std::weak_ptr<WndClass> defaultWndClass;
			if(defaultWndClass.expired()) {
				defaultWndClass = m_wndClass = std::make_shared<WndClass>("reni.window", wndProc);
			} else {
				m_wndClass = defaultWndClass.lock();
			}

			// create a window handle
			m_handle.reset(safeApiCall(CreateWindowEx(
				0, // no extended style
				MAKEINTATOM(m_wndClass->atom()),
				nullptr, // no title
				WS_OVERLAPPEDWINDOW, // simple overlapped window
				CW_USEDEFAULT, // default x-position
				CW_USEDEFAULT, // default y-position
				CW_USEDEFAULT, // default width
				CW_USEDEFAULT, // default height
				nullptr, // no parent
				nullptr, // no menu
				nullptr, // current module
				nullptr // no parameter for WM_CREATE
			)));

			// bind Window object to the native HWND handle
			safeApiCall(SetWindowLongPtr(m_handle.get(), GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this)));
		}


		void installCallbacks(WindowCallbacks* callbacks) override {
			m_callbacks = callbacks;
		}


		void setTitle(std::string_view newTitle) override {
			const auto tcTitle = mbToTc(newTitle);
			safeApiCall(SetWindowText(
				m_handle.get(), tcTitle.c_str()
			));
		}


		void setClientSize(Size2D newSize) override {
			// calculate the window size with the desired client area size
			const auto style = safeApiCall(GetWindowLongPtr(m_handle.get(), GWL_STYLE));
			const auto styleEx = safeApiCall(GetWindowLongPtr(m_handle.get(), GWL_EXSTYLE));
			const auto hasMenu = safeApiCall(GetMenu(m_handle.get())) != nullptr;

			RECT rc = { 0, 0, newSize.width, newSize.height };
			safeApiCall(AdjustWindowRectEx(
				&rc,
				static_cast<DWORD>(style),
				hasMenu,
				static_cast<DWORD>(styleEx)
			));

			// update the window size
			safeApiCall(SetWindowPos(
				m_handle.get(),
				nullptr,
				0, 0, // no need to move the window
				rc.right - rc.left, rc.bottom - rc.top, // new window size
				SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER
			));
		}


		void setVisible(bool visible) override {
			safeApiCall(ShowWindow(m_handle.get(), visible ? SW_SHOW : SW_HIDE));
		}


		std::string getTitle() const override {
			const auto tcTitleLen = safeApiCall(GetWindowTextLength(m_handle.get())) + 1;
			if(tcTitleLen > 1) {
				TCharString tcTitle(tcTitleLen, '\0');
				safeApiCall(GetWindowText(
					m_handle.get(), tcTitle.data(), tcTitleLen
				));
				tcTitle.pop_back(); // remove the null character
				return tcToMb(tcTitle);
			}
			return "";
		}


		Size2D getClientSize() const override {
			RECT r = {};
			safeApiCall(GetClientRect(m_handle.get(), &r));
			return { r.right, r.bottom };
		}


		bool isVisible() const override {
			return static_cast<bool>(safeApiCall(IsWindowVisible(m_handle.get())));
		}


		Point2D getMousePos() const override {
			POINT p = {};
			safeApiCall(GetCursorPos(&p));
			safeApiCall(ScreenToClient(m_handle.get(), &p));
			return { p.x, p.y };
		}


		void* nativeHandle() const override {
			return m_handle.get();
		}


	private:
		static LRESULT wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
			if(const auto window = fromHandle(hwnd)) {
				// translate message codes into the appropriate events
				switch(msg) {
					// window messages
					case WM_SIZE:
						return window->performCallback(&WindowCallbacks::onWindowResize, Size2D(LOWORD(lParam), HIWORD(lParam)));

					case WM_CLOSE:
						return window->performCallback(&WindowCallbacks::onWindowClose);

					// keyboard messages
					case WM_KEYDOWN: {
						if(!(lParam & 0x40000000)) { // do not repeat the notification if the key is held down
							if(const auto key = mapVkeyToKeys(wParam)) {
								return window->performCallback(&WindowCallbacks::onKeyStateChange, *key, true);
							}
						}
						break;
					}

					case WM_KEYUP: {
						if(const auto key = mapVkeyToKeys(wParam)) {
							return window->performCallback(&WindowCallbacks::onKeyStateChange, *key, false);
						}
						break;
					}

					// mouse messages
					case WM_MOUSEMOVE:
						return window->performCallback(&WindowCallbacks::onMouseMove, Point2D(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));

					case WM_LBUTTONDOWN: case WM_LBUTTONUP:
						return window->performCallback(&WindowCallbacks::onMouseButtonStateChange, MouseButtons::Left, msg == WM_LBUTTONDOWN);

					case WM_RBUTTONDOWN: case WM_RBUTTONUP:
						return window->performCallback(&WindowCallbacks::onMouseButtonStateChange, MouseButtons::Right, msg == WM_RBUTTONDOWN);

					case WM_MBUTTONDOWN: case WM_MBUTTONUP:
						return window->performCallback(&WindowCallbacks::onMouseButtonStateChange, MouseButtons::Middle, msg == WM_MBUTTONDOWN);
				}
			}
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
		

		static std::optional<Keys> mapVkeyToKeys(WPARAM vkey) noexcept {
			switch(vkey) {
				case VK_LEFT: return Keys::LeftArrow;
				case VK_RIGHT: return Keys::RightArrow;
				case VK_UP: return Keys::UpArrow;
				case VK_DOWN: return Keys::DownArrow;
			}
			return std::nullopt;
		}
		

		static Win32Window* fromHandle(HWND handle) {
			if(handle) {
				return reinterpret_cast<Win32Window*>(safeApiCall(GetWindowLongPtr(handle, GWLP_USERDATA)));
			}
			return nullptr;
		}


		template <typename... Args, typename... Types>
			requires (std::convertible_to<Args, Types> && ...)
		LRESULT performCallback(void (WindowCallbacks::* callback)(Types...), Args&&... args) {
			if(m_callbacks) {
				(m_callbacks->*callback)(std::forward<Args>(args)...);
			}
			return 0;
		}


		class HwndDeleter {
		public:
			using pointer = HWND;
			
			void operator()(pointer handle) {
				safeApiCall(DestroyWindow(handle));
			}
		};

		std::shared_ptr<WndClass> m_wndClass;
		std::unique_ptr<HWND, HwndDeleter> m_handle;
		WindowCallbacks* m_callbacks = {};
	};

}

#endif