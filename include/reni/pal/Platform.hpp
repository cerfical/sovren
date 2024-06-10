#pragma once

#include "../rhi/RenderBackend.hpp"

#include "../util/NonCopyable.hpp"
#include "../util/NonMovable.hpp"

#include "EventPoller.hpp"
#include "Window.hpp"

#include <memory>

namespace reni::pal {

	class Platform : private NonCopyable, private NonMovable {
	public:

		static Platform* get();


		virtual std::unique_ptr<Window> createWindow() = 0;

		virtual std::unique_ptr<EventPoller> createEventPoller() = 0;
		
		virtual std::unique_ptr<rhi::RenderBackend> createRenderBackend() = 0;
		
	};

}