#pragma once

#include "../util/types.hpp"

namespace reni::pal {

	class EventPoller : private NonCopyable, private NonMovable {
	public:
		virtual ~EventPoller() = default;


		virtual bool pollEvents() = 0;
		
		virtual void waitEvents() = 0;

	};

}