#include "WindowImplBase.hpp"
#include "RenderEngine.hpp"

#include "WndClass.hpp"
#include "WinUtils.hpp"

#include <optional>
#include <gsl/util>

#include <windowsx.h>


namespace {

#define RENI_VKEY_MAPPING_LIST \
	RENI_VKEY_MAPPING(VK_LEFT, LeftArrow) \
	RENI_VKEY_MAPPING(VK_RIGHT, RightArrow) \
	RENI_VKEY_MAPPING(VK_UP, UpArrow) \
	RENI_VKEY_MAPPING(VK_DOWN, DownArrow)


	std::optional<RENI::Keys> mapVKeyToKeys(WPARAM vkey) noexcept {

#define RENI_VKEY_MAPPING(vkey, key) case vkey: { return RENI::Keys::key; }
		switch(vkey) {
			RENI_VKEY_MAPPING_LIST
		}
#undef RENI_VKEY_MAPPING

		return { };
	}

}

struct RENI::Window::Impl : ImplBase {
	/** @{ */
	static Impl* fromHandle(HWND handle) {
		if(handle) {
			const auto impl = reinterpret_cast<Impl*>(
				safeWin32ApiCall(GetWindowLongPtr, handle, GWLP_USERDATA)
			);
			return impl;
		}
		return nullptr;
	}

	static LRESULT wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
		static int visibleWindows = 0;
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
				case WM_SHOWWINDOW: {
					if(wParam == TRUE) {
						visibleWindows++;
					} else if (wParam == FALSE) {
						visibleWindows--;
						if(visibleWindows == 0) {
							PostQuitMessage(0);
						}
					}
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
	/** @} */


	/** @{ */
	explicit Impl(Window& window) : ImplBase(window) {
		// initialize a new Win32 window class
		static std::weak_ptr<WndClass> wndClass;
		if(wndClass.expired()) {
			wndClass = this->wndClass = std::make_shared<WndClass>("RENI.Window", wndProc);
		} else {
			this->wndClass = wndClass.lock();
		}
		
		// create window handle
		handle.reset(safeWin32ApiCall(CreateWindowEx,
			0, // no extended style
			MAKEINTATOM(this->wndClass->atom()),
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
		));

		// bind WinWindow object to the native HWND handle
		safeWin32ApiCall(SetWindowLongPtr,
			handle.get(), GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this)
		);

		// set the initial window size
		RECT rect = { };
		safeWin32ApiCall(GetClientRect, handle.get(), &rect);
		clientSize = Size2D(
			gsl::narrow_cast<int>(rect.right),
			gsl::narrow_cast<int>(rect.bottom)
		);
		
		// set the initial position of the mouse cursor
		POINT cursorPos = { };
		safeWin32ApiCall(::GetCursorPos, &cursorPos);
		safeWin32ApiCall(ScreenToClient, handle.get(), &cursorPos);
		this->cursorPos = Point2D(
			gsl::narrow_cast<int>(cursorPos.x),
			gsl::narrow_cast<int>(cursorPos.y)
		);

		device = RenderEngine::get()->createWindowDevice(handle.get());
	}
	/** @} */


	/** @{ */
	class HwndDeleter {
	public:
		using pointer = HWND;
		void operator()(pointer handle) {
			safeWin32ApiCall(DestroyWindow, handle);
		}
	};
	/** @} */

	/** @{ */
	std::shared_ptr<WndClass> wndClass;
	std::unique_ptr<HWND, HwndDeleter> handle;
	std::unique_ptr<RenderDevice> device;
	/** @} */
};


namespace RENI {
	void Window::onClose() {
		hide();
	}

	void Window::onResize(const Size2D& newSize, const Size2D& oldSize) {
		renderDevice()->resize(newSize);
	}


	Window::Window() {
		m_impl = std::make_unique<Impl>(*this);
		renderDevice()->resize(size());
	}

	Window::~Window() = default;


	void Window::resize(const Size2D& size) {
		if(m_impl->clientSize != size) {
			safeWin32ApiCall(SetWindowPos,
				m_impl->handle.get(),
				nullptr,
				0,
				0,
				size.width(),
				size.height(),
				SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER
			);
		}
	}

	const Size2D& Window::size() const {
		return m_impl->clientSize;
	}


	void Window::setTitle(std::string_view title) {
		const auto tcTitle = mbToTc(title);
		safeWin32ApiCall(
			SetWindowText, m_impl->handle.get(), tcTitle.c_str()
		);
	}

	std::string Window::title() const {
		const auto tcTitleLen = safeWin32ApiCall(GetWindowTextLength, m_impl->handle.get()) + 1;
		if(tcTitleLen > 1) {
			TString tcTitle(tcTitleLen, { });
			safeWin32ApiCall(GetWindowText,
				m_impl->handle.get(), tcTitle.data(), tcTitleLen
			);
			tcTitle.pop_back(); // remove the null character
			return tcToMb(tcTitle);
		}
		return { };
	}


	void Window::setVisible(bool visible) {
		safeWin32ApiCall(ShowWindow,
			m_impl->handle.get(), visible ? SW_SHOW : SW_HIDE
		);
	}

	bool Window::visible() const {
		return gsl::narrow_cast<bool>(safeWin32ApiCall(
			IsWindowVisible, m_impl->handle.get()
		));
	}


	bool Window::keyState(Keys k) const {
		return m_impl->keyState(k);
	}

	bool Window::buttonState(MouseButtons b) const {
		return m_impl->buttonState(b);
	}
	
	void Window::toggleMouseCapture() {
		if(safeWin32ApiCall(GetCapture) == m_impl->handle.get()) {
			safeWin32ApiCall(ReleaseCapture);
		} else {
			safeWin32ApiCall(SetCapture, m_impl->handle.get());
		}
	}


	void* Window::nativeHandle() const {
		return m_impl->handle.get();
	}


	RenderDevice* Window::renderDevice() const {
		return m_impl->device.get();
	}
}