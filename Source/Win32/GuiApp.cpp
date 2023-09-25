#include "GuiApp.hpp"

#include "WinUtils.hpp"
#include "WndProc.hpp"

namespace RENI {
	using namespace Win32;

	int GuiApp::Exec() {
		MSG msg = { };
		while(SafeWin32ApiCall(GetMessage, &msg, nullptr, 0, 0)
			&& WndProc::WindowsVisible() > 0
		) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			WndProc::RethrowExceptions();
		}
		return 0;
	}
}