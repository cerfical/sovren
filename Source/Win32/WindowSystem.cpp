#include "WinWindowSystem.hpp"

namespace RENI {
	WindowSystem* WindowSystem::instance() {
		return WinWindowSystem::instance();
	}
}