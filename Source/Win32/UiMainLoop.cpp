#include "UiMainLoop.hpp"

#include "Win32Utils.hpp"
#include <gsl/util>

struct RENI::UiMainLoop::Impl {
	MSG lastMsg = { };
	bool active = true;
};

namespace RENI {
	UiMainLoop* UiMainLoop::get() {
		static UiMainLoop instance;
		return &instance;
	}


	UiMainLoop::UiMainLoop() : m_impl(std::make_unique<Impl>())
	{ }

	UiMainLoop::~UiMainLoop() = default;


	bool UiMainLoop::active() const {
		return m_impl->active;
	}

	bool UiMainLoop::anyEvents() const {
		if(win32Check(PeekMessage(&m_impl->lastMsg, NULL, 0, 0, PM_NOREMOVE))) {
			return true;
		}
		return false;
	}

	int UiMainLoop::exitCode() const {
		if(active()) {
			return 0;
		}
		return gsl::narrow_cast<int>(m_impl->lastMsg.wParam);
	}


	void UiMainLoop::waitEvents() const {
		if(active()) {
			win32Check(WaitMessage());
		}
	}

	void UiMainLoop::pollEvents() {
		while(active() && PeekMessage(&m_impl->lastMsg, NULL, 0, 0, PM_REMOVE)) {
			if(m_impl->lastMsg.message == WM_QUIT) {
				m_impl->active = false;
				break;
			}
			TranslateMessage(&m_impl->lastMsg);
			DispatchMessage(&m_impl->lastMsg);
		}
	}


	void UiMainLoop::exit(int code) {
		PostQuitMessage(code);
	}

	int UiMainLoop::exec() {
		while(active()) {
			pollEvents();
			waitEvents();
		}
		return exitCode();
	}
}