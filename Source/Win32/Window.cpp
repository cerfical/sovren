#include "WindowImplBase.hpp"

#include "WndClass.hpp"
#include "utils.hpp"

#include <optional>
#include <gsl/narrow>

#include <windowsx.h>

namespace reni {

	namespace {

#define RENI_VKEY_MAPPING_LIST \
	RENI_VKEY_MAPPING(VK_LEFT, LeftArrow) \
	RENI_VKEY_MAPPING(VK_RIGHT, RightArrow) \
	RENI_VKEY_MAPPING(VK_UP, UpArrow) \
	RENI_VKEY_MAPPING(VK_DOWN, DownArrow)

		std::optional<Keys> mapVKeyToKeys(WPARAM vkey) noexcept {
#define RENI_VKEY_MAPPING(vkey, key) case vkey: { return Keys::key; }
			switch(vkey) {
				RENI_VKEY_MAPPING_LIST
			}
#undef RENI_VKEY_MAPPING
			return {};
		}

	}

	struct Window::Impl : ImplBase {
		
		static Impl* fromHandle(HWND handle) {
			if(handle) {
				const auto impl = reinterpret_cast<Impl*>(
					win32Check(GetWindowLongPtr(handle, GWLP_USERDATA))
				);
				return impl;
			}
			return nullptr;
		}

		static LRESULT wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
			if(const auto impl = fromHandle(hwnd)) {
				// translate message codes into the appropriate events
				switch(msg) {
					// window messages
					case WM_SIZE: {
						const auto newSize = Size2D(LOWORD(lParam), HIWORD(lParam));
						impl->updateSize(newSize);
						return 0;
					}
					case WM_CLOSE: {
						impl->window.onClose();
						return 0;
					}

					// keyboard messages
					case WM_KEYDOWN: {
						if(!(lParam & 0x40000000)) { // do not repeat the notification if the key is held down
							if(const auto key = mapVKeyToKeys(wParam); key) {
								impl->pressKey(*key);
							}
						}
						return 0;
					}
					case WM_KEYUP: {
						if(const auto key = mapVKeyToKeys(wParam); key) {
							impl->releaseKey(*key);
						}
						return 0;
					}

					// mouse messages
					case WM_LBUTTONDOWN: return (impl->pressButton(MouseButtons::Left), 0);
					case WM_LBUTTONUP: return (impl->releaseButton(MouseButtons::Left), 0);

					case WM_RBUTTONDOWN: return (impl->pressButton(MouseButtons::Right), 0);
					case WM_RBUTTONUP: return (impl->releaseButton(MouseButtons::Right), 0);

					case WM_MBUTTONDOWN: return (impl->pressButton(MouseButtons::Middle), 0);
					case WM_MBUTTONUP: return (impl->releaseButton(MouseButtons::Middle), 0);

					case WM_MOUSEMOVE: {
						const auto newPos = Point2D(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
						impl->updateCursor(newPos);
						return 0;
					}
				}
			}
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}


		explicit Impl(Window& window) : ImplBase(window) {
			// initialize a new Win32 window class
			static std::weak_ptr<WndClass> wndClassInstance;
			if(wndClassInstance.expired()) {
				wndClassInstance = wndClass = std::make_shared<WndClass>("RENI.Window", wndProc);
			} else {
				wndClass = wndClassInstance.lock();
			}

			// create a window handle
			handle.reset(win32Check(CreateWindowEx(
				0, // no extended style
				MAKEINTATOM(this->wndClass->atom()),
				NULL, // no title
				WS_OVERLAPPEDWINDOW, // simple overlapped window
				CW_USEDEFAULT, // default x-position
				CW_USEDEFAULT, // default y-position
				CW_USEDEFAULT, // default width
				CW_USEDEFAULT, // default height
				NULL, // no parent
				NULL, // no menu
				NULL, // current module
				NULL // no parameter for WM_CREATE
			)));

			// bind Window object to the native HWND handle
			win32Check(SetWindowLongPtr(
				handle.get(), GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this)
			));

			// set the initial window size
			RECT rc = { };
			win32Check(GetClientRect(handle.get(), &rc));
			clientSize = Size2D(
				gsl::narrow_cast<int>(rc.right),
				gsl::narrow_cast<int>(rc.bottom)
			);

			// set the initial position of the mouse cursor
			POINT p = { };
			win32Check(GetCursorPos(&p));
			win32Check(ScreenToClient(handle.get(), &p));
			cursorPos = Point2D(
				gsl::narrow_cast<int>(p.x),
				gsl::narrow_cast<int>(p.y)
			);
		}


		class HwndDeleter {
		public:
			using pointer = HWND;
			void operator()(pointer handle) {
				win32Check(DestroyWindow(handle));
			}
		};


		std::shared_ptr<WndClass> wndClass;
		std::unique_ptr<HWND, HwndDeleter> handle;
	};


	void Window::onClose() {
		hide();
		PostQuitMessage(0);
	}


	Window::Window() : m_impl(std::make_unique<Impl>(*this))
	{ }

	Window::~Window() = default;


	void Window::setSize(Size2D s) {
		// calculate the window size with the desired client area size
		const auto style = win32Check(GetWindowLongPtr(m_impl->handle.get(), GWL_STYLE));
		const auto styleEx = win32Check(GetWindowLongPtr(m_impl->handle.get(), GWL_EXSTYLE));
		const auto hasMenu = win32Check(GetMenu(m_impl->handle.get())) != NULL;

		RECT rc = { 0, 0, gsl::narrow_cast<LONG>(s.width), gsl::narrow_cast<LONG>(s.height) };
		win32Check(AdjustWindowRectEx(
			&rc,
			gsl::narrow_cast<DWORD>(style),
			hasMenu,
			gsl::narrow_cast<DWORD>(styleEx)
		));

		// update the window size
		win32Check(SetWindowPos(
			m_impl->handle.get(),
			NULL,
			0, 0, // no need to move the window
			rc.right - rc.left, rc.bottom - rc.top, // new window size
			SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER
		));
	}

	Size2D Window::size() const {
		return m_impl->clientSize;
	}


	void Window::setTitle(std::string_view title) {
		const auto tcTitle = mbToTc(title);
		win32Check(SetWindowText(
			m_impl->handle.get(), tcTitle.c_str()
		));
	}

	std::string Window::title() const {
		const auto tcTitleLen = win32Check(GetWindowTextLength(m_impl->handle.get())) + 1;
		if(tcTitleLen > 1) {
			tstring tcTitle(tcTitleLen, { });
			win32Check(GetWindowText(
				m_impl->handle.get(), tcTitle.data(), tcTitleLen
			));
			tcTitle.pop_back(); // remove the null character
			return tcToMb(tcTitle);
		}
		return { };
	}


	void Window::setVisible(bool visible) {
		win32Check(ShowWindow(
			m_impl->handle.get(), visible ? SW_SHOW : SW_HIDE
		));
	}

	bool Window::isVisible() const {
		return gsl::narrow_cast<bool>(win32Check(
			IsWindowVisible(m_impl->handle.get())
		));
	}


	bool Window::keyState(Keys k) const {
		return m_impl->keyState(k);
	}

	bool Window::buttonState(MouseButtons b) const {
		return m_impl->buttonState(b);
	}
	
	void Window::toggleMouseCapture() {
		if(win32Check(GetCapture()) == m_impl->handle.get()) {
			win32Check(ReleaseCapture());
		} else {
			win32Check(SetCapture(m_impl->handle.get()));
		}
	}


	void* Window::nativeHandle() const {
		return m_impl->handle.get();
	}
}