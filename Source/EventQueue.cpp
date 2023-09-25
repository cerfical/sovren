#include "WindowSystem.hpp"

namespace RENI {
	EventQueue* EventQueue::instance() {
		return WindowSystem::instance()->eventQueue();
	}
}