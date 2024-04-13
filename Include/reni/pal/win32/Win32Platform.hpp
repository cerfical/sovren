#ifndef RENI_PAL_WIN32_PLATFORM_HEADER
#define RENI_PAL_WIN32_PLATFORM_HEADER

#include "../Platform.hpp"

#include "../dx/DxRenderBackend.hpp"

#include "Win32EventPoller.hpp"
#include "Win32Window.hpp"

namespace reni::pal::win32 {

	class Win32Platform : public Platform {
	public:

		std::unique_ptr<Window> createWindow() override {
			return std::make_unique<Win32Window>();
		}

		std::unique_ptr<EventPoller> createEventPoller() override {
			return std::make_unique<Win32EventPoller>();
		}

		std::unique_ptr<RenderBackend> createRenderBackend() override {
			return std::make_unique<dx::DxRenderBackend>();
		}

	};
}

#endif