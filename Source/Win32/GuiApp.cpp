#include "GuiApp.hpp"

#include "WinMsgQueue.hpp"
#include "WinWindow.hpp"

#include <gsl/util>

namespace RENI {
	class GuiApp::Impl : public ImplBase {
	public:
		WinMsgQueue queue;
	};
}

namespace RENI {
	GuiApp::GuiApp() {
		m_impl.Init(std::make_unique<Impl>());
	}

	int GuiApp::Exec() {
		while(true) {
			// retrieve the next message from the system and if the message is a quit signal, exit the application
			const auto msg = m_impl->queue.GetMsg();
			if(msg.message == WM_QUIT) {
				break;
			}

			// otherwise, if it is the window message, let the window handle it
			if(auto wnd = WinWindow::FromHandle(msg.hwnd)) {
				wnd->HandleMsg(msg);
			}
		}
		// return the exit code from the WM_QUIT message that ended the loop
		return gsl::narrow_cast<int>(m_impl->queue.GetLastMsg().wParam);
	}
}