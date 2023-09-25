#include "WinWindowSystem.hpp"

#include "WinEventQueue.hpp"
#include "WinWindow.hpp"
#include "WinUtils.hpp"

namespace RENI {
	WinWindowSystem* WinWindowSystem::instance() noexcept {
		static WinWindowSystem windowSystem;
		return &windowSystem;
	}

	Window* WinWindowSystem::windowFromHandle(HWND handle) {
		if(handle) {
			const auto windowHandle = reinterpret_cast<WinWindow*>(
				safeWin32ApiCall(GetWindowLongPtr, handle, GWLP_USERDATA)
			);
			return windowHandle->attachedWindow();
		}
		return nullptr;
	}

	WindowHandle* WinWindowSystem::createWindow() {
		auto window = std::make_unique<WinWindow>();
		safeWin32ApiCall(SetWindowLongPtr,
			reinterpret_cast<HWND>(window->nativeHandle()), GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window.get())
		); // bind WindowHandle object to the native HWND handle
		return window.release();
	}

	EventQueue* WinWindowSystem::eventQueue() {
		return WinEventQueue::instance();
	}
}