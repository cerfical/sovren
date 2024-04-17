#ifndef RENI_PAL_PLATFORM_HEADER
#define RENI_PAL_PLATFORM_HEADER

#include "../utils.hpp"
#include "../rhi/RenderBackend.hpp"

#include "Window.hpp"
#include "EventPoller.hpp"

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

#endif