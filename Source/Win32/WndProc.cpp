#include "WndProc.hpp"

#include "WinWindow.hpp"
#include <exception>

namespace {
	using namespace RENI::Win32;

	struct WndProcState {
		std::exception_ptr exception;
		bool inSizeMove = false;
		int windowsVisible= 0;
	} state;

	LRESULT CALLBACK DefWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
		switch(msg) {
			case WM_WINDOWPOSCHANGING: {
				if(state.inSizeMove && WndProc::AnyExceptions()) {
					const auto wndPos = reinterpret_cast<WINDOWPOS*>(lParam);
					// try to exit the resize/move loop and return to the main message loop
					if(hwnd == GetCapture()) {
						ReleaseCapture();
					}
					wndPos->flags |= SWP_NOSIZE | SWP_NOMOVE; // prevent the window from being resized/moved
				}
				break;
			}
			case WM_ENTERSIZEMOVE: {
				state.inSizeMove = true;
				break;
			}
			case WM_EXITSIZEMOVE: {
				state.inSizeMove = false;
				break;
			}
			case WM_SHOWWINDOW: {
				if(wParam) {
					state.windowsVisible++;
				} else {
					state.windowsVisible--;
				}
				break;
			}
		}

		// forward the message to WinWindow for further processing
		try {
			const auto window = WinWindow::FromHandle(hwnd);
			if(window && !WndProc::AnyExceptions()) {
				return window->HandleMessage(msg, wParam, lParam);
			}
		} catch(...) {
			state.exception = std::current_exception();
			return 0;
		}
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

namespace RENI::Win32 {
	bool WndProc::AnyExceptions() noexcept {
		return state.exception != nullptr;
	}
	
	void WndProc::RethrowExceptions() {
		if(AnyExceptions()) {
			std::rethrow_exception(std::exchange(
				state.exception, nullptr
			));
		}
	}

	int WndProc::WindowsVisible() noexcept {
		return state.windowsVisible;
	}

	WNDPROC WndProc::Get() noexcept {
		return DefWndProc;
	}
}