#include "Win32BackendFactory.hpp"

namespace RENI {
	BackendFactory* BackendFactory::get() {
		static Win32BackendFactory instance;
		return &instance;
	}
}