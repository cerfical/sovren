#include "pal/win32/Win32Platform.hpp"

namespace reni::pal {
	Platform* Platform::get() {
		static win32::Win32Platform platform;
		return &platform;
	}
}