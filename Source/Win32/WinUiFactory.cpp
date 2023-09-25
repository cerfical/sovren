#include "D2DUiFactory.hpp"

namespace RENI {
	WinUiFactory* WinUiFactory::Get() {
		static D2DUiFactory factory;
		return &factory;
	}
}