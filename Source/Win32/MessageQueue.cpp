#include "MessageQueue.hpp"
#include "WinUtils.hpp"

#include <gsl/util>

namespace RENI {
	void MessageQueue::DispatchMessages() {
		while(PeekMessage(&m_msg, nullptr, 0, 0, PM_REMOVE)) {
			if(m_msg.message != WM_QUIT) {
				TranslateMessage(&m_msg);
				DispatchMessage(&m_msg);
			}
		}
	}

	void MessageQueue::Wait() const {
		SafeWin32ApiCall(WaitMessage);
	}


	void MessageQueue::PostQuitMessage(int exitCode) {
		SafeWin32ApiCall(::PostQuitMessage, exitCode);
	}

	bool MessageQueue::IsQuitMessage() const noexcept {
		return m_msg.message == WM_QUIT;
	}


	int MessageQueue::GetExitCode() const noexcept {
		if(m_msg.message == WM_QUIT) {
			return gsl::narrow_cast<int>(m_msg.wParam);
		}
		return 0;
	}
}