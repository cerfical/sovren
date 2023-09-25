#include "Ui.hpp"

#include <Windows.h>
#include <gsl/util>

namespace RENI {
	int Ui::eventLoop() {
		MSG msg = { };
		while(msg.message != WM_QUIT) {
			if(GetMessage(&msg, nullptr, 0, 0) == -1) {
				return 1;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return gsl::narrow_cast<int>(msg.wParam);
	}
}