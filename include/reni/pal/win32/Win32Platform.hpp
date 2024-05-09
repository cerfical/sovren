#pragma once

#include "../Platform.hpp"

namespace reni::pal::win32 {

	class Win32Platform : public Platform {
	public:

		std::unique_ptr<Window> createWindow() override;

		std::unique_ptr<EventPoller> createEventPoller() override;

		std::unique_ptr<rhi::RenderBackend> createRenderBackend() override;

	};
}