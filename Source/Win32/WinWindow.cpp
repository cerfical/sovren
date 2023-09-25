#include "WinWindow.hpp"
#include "Log.hpp"

#include <gsl/util>

namespace RENI {
	std::exception_ptr WinWindow::wndProcException;

	WinWindow* WinWindow::FromHandle(HWND handle) {
		if(handle) {
			return reinterpret_cast<WinWindow*>(
				SafeWndProcCall(GetWindowLongPtr, handle, GWLP_USERDATA)
			);
		}
		return nullptr;
	}

	LRESULT CALLBACK WinWindow::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
		static bool inSizeMove = false;
		static int visibleWindows = 0;

		switch(msg) {
			case WM_ENTERSIZEMOVE: { inSizeMove = true; break; }
			case WM_EXITSIZEMOVE: { inSizeMove = false; break; }
			case WM_SHOWWINDOW: {
				if(wParam) {
					visibleWindows++;
				} else {
					visibleWindows--;
					if(visibleWindows == 0) {
						PostQuitMessage(0);
					}
				}
				break;
			}
		}

		// forward the message to the corresponding window for further processing
		try {
			const auto window = WinWindow::FromHandle(hwnd);
			if(window && !wndProcException) {
				return window->OnMessage(hwnd, msg, wParam, lParam);
			}
			return DefWindowProc(hwnd, msg, wParam, lParam);
		} catch(...) {
			wndProcException = std::current_exception();
			// if in a size/move modal loop, try to end it
			if(inSizeMove) {
				PostMessage(hwnd, WM_LBUTTONUP, 0, 0);
			}
		}
		return 0;
	}


	const WinWndClass* WinWindow::GetWndClass() {
		static WinWndClass wndClass("RENI.WinWindow", WndProc);
		return &wndClass;
	}

	HWND WinWindow::CreateHWnd() {
		const auto wndClass = GetWndClass();
		return SafeWin32ApiCall(CreateWindowEx,
			0, MAKEINTATOM(wndClass->GetAtom()), nullptr, WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
			nullptr, nullptr, nullptr, nullptr
		);
	}

	void WinWindow::HwndDeleter::operator()(pointer handle) {
		SafeWndProcCall(DestroyWindow, handle);
		Log::Info("Window *:{0:#x} destroyed", reinterpret_cast<std::uintptr_t>(handle));
	}

	WinWindow::WinWindow() {
		m_handle.reset(CreateHWnd());

		SafeWndProcCall(SetWindowLongPtr,
			m_handle.get(), GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this)
		); // bind "this" to the HWND

		Log::Info("Window *:{0:#x} created", reinterpret_cast<std::uintptr_t>(m_handle.get()));
	}


	void WinWindow::SetClientArea(const Extent2D& clientArea) {
		SafeWndProcCall(SetWindowPos,
			GetHandle(), nullptr, 0, 0, clientArea.width, clientArea.height,
			SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER
		);
	}

	Extent2D WinWindow::GetClientArea() const {
		RECT rect = { };
		SafeWndProcCall(GetClientRect, GetHandle(), &rect);
		return {
			gsl::narrow_cast<int>(rect.right),
			gsl::narrow_cast<int>(rect.bottom)
		};
	}

	void WinWindow::SetTitle(std::string_view title) {
		const auto tcTitle = MbToTc(title);
		SafeWndProcCall(
			SetWindowText, GetHandle(), tcTitle.c_str()
		);
	}

	std::string WinWindow::GetTitle() const {
		const auto tcTitleLen = SafeWndProcCall(GetWindowTextLength, GetHandle()) + 1;
		if(tcTitleLen > 1) {
			TString tcTitle(tcTitleLen, { });
			SafeWndProcCall(GetWindowText,
				GetHandle(), tcTitle.data(), tcTitleLen
			);
			tcTitle.pop_back(); // remove the null character
			return TcToMb(tcTitle);
		}
		return { };
	}

	void WinWindow::SetVisible(bool visible) {
		SafeWndProcCall(ShowWindow,
			GetHandle(), visible ? SW_SHOW : SW_HIDE
		);
	}

	bool WinWindow::IsVisible() const {
		return static_cast<bool>(SafeWndProcCall(
			IsWindowVisible, GetHandle()
		));
	}


	Point2D WinWindow::GetCursorPos() const {
		POINT cursorPos = { };
		SafeWin32ApiCall(::GetCursorPos, &cursorPos);
		SafeWndProcCall(ScreenToClient, GetHandle(), &cursorPos);
		return {
			gsl::narrow_cast<int>(cursorPos.x),
			gsl::narrow_cast<int>(cursorPos.y)
		};
	}

	void WinWindow::CaptureMouse() {
		SafeWndProcCall(SetCapture, GetHandle());
	}

	void WinWindow::ReleaseMouse() {
		if(SafeWndProcCall(GetCapture) == GetHandle()) {
			SafeWndProcCall(ReleaseCapture);
		}
	}


	void WinWindow::HandleMsg(const MSG& msg) {
		SafeWndProcCall(TranslateMessage, &msg);
		SafeWndProcCall(DispatchMessage, &msg);
	}
}