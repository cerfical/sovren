#include "Ui.hpp"

#include "WinUtils.hpp"
#include "WndProc.hpp"

#include <Windows.h>

namespace RENI {
	int Ui::EnterUiLoop() {
		MSG msg = { };
		while(Win32::SafeWin32ApiCall(GetMessage, &msg, nullptr, 0, 0)
			&& Win32::WndProc::WindowsVisible() > 0
		) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return 0;
	}
}