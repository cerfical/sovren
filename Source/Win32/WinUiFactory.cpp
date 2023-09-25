#include "D2D/D2DUiFactory.hpp"

namespace RENI::Win32 {
	std::shared_ptr<WinUiFactory> WinUiFactory::Get() {
		// possible race condition in a multithreaded environment
		static std::weak_ptr<WinUiFactory> factory;
		if(factory.expired()) {
			const auto newFactory = std::make_shared<D2D::D2DUiFactory>();
			factory = newFactory;
			return newFactory;
		}
		return factory.lock();
	}
}