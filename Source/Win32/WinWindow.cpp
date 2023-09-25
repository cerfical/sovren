#include "WinWindow.hpp"

#include "WinEventQueue.hpp"
#include "WinUtils.hpp"
#include "Log.hpp"

#include <gsl/util>

namespace RENI {
	std::shared_ptr<WinWndClass> WinWindow::wndClass() {
		static std::weak_ptr<WinWndClass> wndClass;
		if(wndClass.expired()) {
			auto ptr = std::make_shared<WinWndClass>("RENI.WinWindow", WinEventQueue::wndProc);
			wndClass = ptr;
			return ptr;
		}
		return wndClass.lock();
	}

	HWND WinWindow::createHwnd(const WinWndClass& wndClass) {
		return safeWin32ApiCall(CreateWindowEx,
			0, // no extended style
			MAKEINTATOM(wndClass.atom()),
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
		);
	}

	void WinWindow::HwndDeleter::operator()(pointer handle) {
		safeWin32ApiCall(DestroyWindow, handle);
		
		Log::info("Window *:{0} destroyed", reinterpret_cast<void*>(handle));
	}


	WinWindow::WinWindow() {
		m_wndClass = wndClass();
		m_handle.reset(createHwnd(*m_wndClass));

		Log::info("Window *:{0} created", reinterpret_cast<void*>(m_handle.get()));
	}


	void WinWindow::setClientSize(const Size2D& size) {
		safeWin32ApiCall(SetWindowPos,
			handle(),
			nullptr,
			0,
			0,
			size.width(),
			size.height(),
			SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER
		);
	}

	Size2D WinWindow::clientSize() const {
		RECT rect = { };
		safeWin32ApiCall(GetClientRect, handle(), &rect);

		return Size2D()
			.width(gsl::narrow_cast<int>(rect.right))
			.height(gsl::narrow_cast<int>(rect.bottom));
	}


	void WinWindow::setTitle(std::string_view title) {
		const auto tcTitle = mbToTc(title);
		safeWin32ApiCall(
			SetWindowText, handle(), tcTitle.c_str()
		);
	}

	std::string WinWindow::title() const {
		const auto tcTitleLen = safeWin32ApiCall(GetWindowTextLength, handle()) + 1;
		if(tcTitleLen > 1) {
			TString tcTitle(tcTitleLen, { });
			safeWin32ApiCall(GetWindowText,
				handle(), tcTitle.data(), tcTitleLen
			);
			tcTitle.pop_back(); // remove the null character
			return tcToMb(tcTitle);
		}
		return { };
	}


	void WinWindow::setVisible(bool visible) {
		safeWin32ApiCall(ShowWindow,
			handle(), visible ? SW_SHOW : SW_HIDE
		);
	}

	bool WinWindow::visible() const {
		return static_cast<bool>(safeWin32ApiCall(
			IsWindowVisible, handle()
		));
	}


	Point2D WinWindow::mousePos() const {
		POINT cursorPos = { };
		safeWin32ApiCall(GetCursorPos, &cursorPos);
		safeWin32ApiCall(ScreenToClient, handle(), &cursorPos);
		return Point2D()
			.x(gsl::narrow_cast<int>(cursorPos.x))
			.y(gsl::narrow_cast<int>(cursorPos.y));
	}

	void WinWindow::setMouseCapture(bool enable) {
		if(enable) {
			safeWin32ApiCall(SetCapture, handle());
		} else {
			if(safeWin32ApiCall(GetCapture) == handle()) {
				safeWin32ApiCall(ReleaseCapture);
			}
		}
	}
}