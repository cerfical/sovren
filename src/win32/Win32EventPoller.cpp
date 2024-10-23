#include "pal/win32/Win32EventPoller.hpp"
#include "pal/win32/util.hpp"

namespace sovren::pal::win32 {
    bool Win32EventPoller::pollEvents() {
        bool anyEvents = false;
        while(PeekMessage(&m_lastMsg, nullptr, 0, 0, PM_REMOVE) != 0) {
            TranslateMessage(&m_lastMsg);
            DispatchMessage(&m_lastMsg);

            anyEvents = true;
        }
        return anyEvents;
    }

    void Win32EventPoller::waitEvents() {
        win32Check(WaitMessage());
    }
}
