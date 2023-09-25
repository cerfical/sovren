#include "WinWindow.hpp"

#include "WinUiFactory.hpp"
#include "WinUtils.hpp"
#include "WndProc.hpp"

#include "Log.hpp"

#include <gsl/util>

namespace {
	using namespace RENI::Win32;

	std::shared_ptr<WndClass> InitWndClass() {
		// possible race condition in a multi-threaded environment
		static std::weak_ptr<WndClass> wndClass;
		if(wndClass.expired()) {
			const auto newClass = std::make_shared<WndClass>("RENI.Window");
			wndClass = newClass;
			return newClass;
		}
		return wndClass.lock();
	}

	HWND InitWindow(const WndClass& wndClass) {
		return SafeWin32ApiCall(CreateWindowEx,
			0, MAKEINTATOM(wndClass.GetAtom()), nullptr, WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
			nullptr, nullptr, nullptr, nullptr
		);
	}
}

namespace RENI::Win32 {
	void WinWindow::HwndDeleter::operator()(pointer handle) {
		SafeWin32ApiCall(DestroyWindow, handle);
		Log::Info("Window *:{0:#x} destroyed", reinterpret_cast<std::uintptr_t>(handle));
	}
	
	WinWindow* WinWindow::FromHandle(HWND handle) {
		return reinterpret_cast<WinWindow*>(
			SafeWin32ApiCall(GetWindowLongPtr,
				handle, GWLP_USERDATA
			)
		);
	}

	WinWindow::WinWindow() {
		wndClass = InitWndClass();
		handle.reset(InitWindow(*wndClass));

		canvas = WinUiFactory::Get()->MakeCanvas(*this);
		
		SafeWin32ApiCall(SetWindowLongPtr,
			handle.get(), GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this)
		); // bind "this" to the window handle
		
		Log::Info("Window *:{0:#x} created", reinterpret_cast<std::uintptr_t>(handle.get()));
	}

	void WinWindow::SetClientArea(Extent2D clientArea) {
		SafeWin32ApiCall(SetWindowPos,
			handle.get(), nullptr, 0, 0,
			clientArea.width, clientArea.height,
			SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER
		);
		WndProc::RethrowExceptions();
	}

	Extent2D WinWindow::GetClientArea() const {
		RECT rect = { };
		SafeWin32ApiCall(GetClientRect, handle.get(), &rect);
		return {
			gsl::narrow_cast<std::size_t>(rect.right),
			gsl::narrow_cast<std::size_t>(rect.bottom)
		};
	}

	void WinWindow::SetTitle(std::string_view title) {
		const auto tcTitle = MbToTc(title);
		SafeWin32ApiCall(
			SetWindowText, handle.get(), tcTitle.c_str()
		);
	}

	std::string WinWindow::GetTitle() const {
		const auto tcTitleLen = SafeWin32ApiCall(GetWindowTextLength, handle.get()) + 1;
		if(tcTitleLen > 1) {
			TString tcTitle(tcTitleLen, { });
			SafeWin32ApiCall(GetWindowText,
				handle.get(), tcTitle.data(), tcTitleLen
			);
			tcTitle.pop_back(); // remove the null character
			return TcToMb(tcTitle);
		}
		return { };
	}

	void WinWindow::SetVisible(bool visible) {
		SafeWin32ApiCall(ShowWindow,
			handle.get(), visible ? SW_SHOW : SW_HIDE
		);
	}

	bool WinWindow::IsVisible() const {
		return static_cast<bool>(IsWindowVisible(handle.get()));
	}
}