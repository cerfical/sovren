#ifndef RENI_PAL_EVENT_POLLER_HEADER
#define RENI_PAL_EVENT_POLLER_HEADER

#include "../util_types.hpp"

namespace reni::pal {

	class EventPoller : private NonCopyable, private NonMovable {
	public:
		virtual ~EventPoller() = default;


		virtual bool pollEvents() = 0;
		
		virtual void waitEvents() = 0;

	};

}

#endif