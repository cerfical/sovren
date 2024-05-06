#pragma once

#include "../Platform.hpp"

#include "../../rhi/dx/DxRenderBackend.hpp"

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

		std::unique_ptr<rhi::RenderBackend> createRenderBackend() override {
			return std::make_unique<rhi::dx::DxRenderBackend>();
		}

	};
}