#include "UiEventLoop.hpp"

#include "Win32Utils.hpp"
#include <gsl/narrow>

struct RENI::UiEventLoop::Impl {
	void processEvents() {
		while(!isStopped && PeekMessage(&lastMsg, NULL, 0, 0, PM_REMOVE)) {
			if(lastMsg.message == WM_QUIT) {
				isStopped = true;
				code = gsl::narrow_cast<int>(lastMsg.wParam);
				return;
			}
			TranslateMessage(&lastMsg);
			DispatchMessage(&lastMsg);
		}
		win32Check(WaitMessage());
	}

	MSG lastMsg = {};
	int code = {};
	bool isStopped = false;
};

namespace RENI {
	UiEventLoop::UiEventLoop()
		: m_impl(std::make_unique<Impl>())
	{ }

	UiEventLoop::~UiEventLoop() = default;

	int UiEventLoop::run() {
		while(!m_impl->isStopped) {
			m_impl->processEvents();
		}
		return m_impl->code;
	}
}