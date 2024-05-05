#ifndef RENI_PAL_WIN32_EVENT_POLLER_HEADER
#define RENI_PAL_WIN32_EVENT_POLLER_HEADER

#include "../EventPoller.hpp"
#include "util.hpp"

#include <Windows.h>

namespace reni::pal::win32 {

	class Win32EventPoller : public EventPoller {
	public:

		bool pollEvents() override {
			bool anyEvents = false;
			while(anyEvents = (PeekMessage(&m_lastMsg, nullptr, 0, 0, PM_REMOVE) != 0)) {
				TranslateMessage(&m_lastMsg);
				DispatchMessage(&m_lastMsg);
			}
			return anyEvents;
		}


		void waitEvents() override {
			safeApiCall(WaitMessage());
		}


	private:
		MSG m_lastMsg = {};
	};

}

#endif