#include "WinWindow.hpp"
#include "WinUtils.hpp"

#include <windowsx.h>

#include <optional>
#include <gsl/util>

namespace {

#define RENI_VKEY_MAPPING_LIST \
	RENI_VKEY_MAPPING(VK_LEFT, LeftArrow) \
	RENI_VKEY_MAPPING(VK_RIGHT, RightArrow) \
	RENI_VKEY_MAPPING(VK_UP, UpArrow) \
	RENI_VKEY_MAPPING(VK_DOWN, DownArrow)


	std::optional<RENI::Keys> MapVKeyToKeys(WPARAM vkey) noexcept {

#define RENI_VKEY_MAPPING(vkey, key) case vkey: { return RENI::Keys::key; }
		switch(vkey) {
			RENI_VKEY_MAPPING_LIST
		}
#undef RENI_VKEY_MAPPING

		return { };
	}
}

namespace RENI {
	LRESULT WinWindow::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
		if(const auto window = FromHandle(hwnd)) {
			// translate message codes into the appropriate events
			switch(msg) {
				// window messages
				case WM_SIZE: {
					const auto newSize = Size2D(LOWORD(lParam), HIWORD(lParam));
					window->OnResize(newSize);
					return 0;
				}
				case WM_CLOSE: {
					window->OnClose();
					return 0;
				}

				// keyboard messages
				case WM_KEYDOWN: {
					if(!(lParam & 0x40000000)) { // do not repeat the notification if the key is held down
						if(const auto key = MapVKeyToKeys(wParam); key) {
							window->OnKeyPress(*key);
						}
					}
					return 0;
				}
				case WM_KEYUP: {
					if(const auto key = MapVKeyToKeys(wParam); key) {
						window->OnKeyRelease(*key);
					}
					return 0;
				}

				// mouse messages
				case WM_LBUTTONDOWN: return (window->OnMousePress(MouseButtons::Left), 0);
				case WM_LBUTTONUP: return (window->OnMouseRelease(MouseButtons::Left), 0);

				case WM_RBUTTONDOWN: return (window->OnMousePress(MouseButtons::Right), 0);
				case WM_RBUTTONUP: return (window->OnMouseRelease(MouseButtons::Right), 0);

				case WM_MBUTTONDOWN: return (window->OnMousePress(MouseButtons::Middle), 0);
				case WM_MBUTTONUP: return (window->OnMouseRelease(MouseButtons::Middle), 0);

				case WM_MOUSEMOVE: {
					const auto newPos = Point2D(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
					window->OnMouseMove(newPos);
					return 0;
				}
			}
		}

		return DefWindowProc(hwnd, msg, wParam, lParam);
	}


	WinWindow* WinWindow::FromHandle(HWND handle) {
		if(handle) {
			const auto window = reinterpret_cast<WinWindow*>(
				SafeWin32ApiCall(GetWindowLongPtr, handle, GWLP_USERDATA)
			);
			return window;
		}
		return nullptr;
	}

	void WinWindow::HwndDeleter::operator()(pointer handle) {
		SafeWin32ApiCall(DestroyWindow, handle);
	}

	WinWindow::WinWindow() {
		// initialize new Win32 window class
		static std::weak_ptr<WndClass> wndClass;
		if(wndClass.expired()) {
			m_wndClass = std::make_shared<WndClass>("RENI.WinWindow", WndProc);
			wndClass = m_wndClass;
		} else {
			m_wndClass = wndClass.lock();
		}

		// create window handle
		m_handle.reset(SafeWin32ApiCall(CreateWindowEx,
			0, // no extended style
			MAKEINTATOM(m_wndClass->GetAtom()),
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
		SafeWin32ApiCall(SetWindowLongPtr,
			reinterpret_cast<HWND>(GetHandle()), GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this)
		);
	}


	void WinWindow::SetClientSize(const Size2D& size) {
		SafeWin32ApiCall(SetWindowPos,
			m_handle.get(),
			nullptr,
			0,
			0,
			size.GetWidth(),
			size.GetHeight(),
			SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER
		);
	}

	Size2D WinWindow::GetClientSize() const {
		RECT rect = { };
		SafeWin32ApiCall(GetClientRect, m_handle.get(), &rect);

		return Size2D(
			gsl::narrow_cast<int>(rect.right),
			gsl::narrow_cast<int>(rect.bottom)
		);
	}


	void WinWindow::SetTitle(std::string_view title) {
		const auto tcTitle = MbToTc(title);
		SafeWin32ApiCall(
			SetWindowText, m_handle.get(), tcTitle.c_str()
		);
	}

	std::string WinWindow::GetTitle() const {
		const auto tcTitleLen = SafeWin32ApiCall(GetWindowTextLength, m_handle.get()) + 1;
		if(tcTitleLen > 1) {
			TString tcTitle(tcTitleLen, { });
			SafeWin32ApiCall(GetWindowText,
				m_handle.get(), tcTitle.data(), tcTitleLen
			);
			tcTitle.pop_back(); // remove the null character
			return TcToMb(tcTitle);
		}
		return { };
	}


	void WinWindow::SetVisible(bool visible) {
		SafeWin32ApiCall(ShowWindow,
			m_handle.get(), visible ? SW_SHOW : SW_HIDE
		);
	}

	bool WinWindow::IsVisible() const {
		return static_cast<bool>(SafeWin32ApiCall(
			IsWindowVisible, m_handle.get()
		));
	}


	void WinWindow::ToggleMouseCapture() {
		if(SafeWin32ApiCall(GetCapture) == m_handle.get()) {
			SafeWin32ApiCall(ReleaseCapture);
		} else {
			SafeWin32ApiCall(SetCapture, m_handle.get());
		}
	}

	Point2D WinWindow::GetCursorPos() const {
		POINT cursorPos = { };
		SafeWin32ApiCall(::GetCursorPos, &cursorPos);
		SafeWin32ApiCall(ScreenToClient, m_handle.get(), &cursorPos);
		return Point2D(
			gsl::narrow_cast<int>(cursorPos.x),
			gsl::narrow_cast<int>(cursorPos.y)
		);
	}
}