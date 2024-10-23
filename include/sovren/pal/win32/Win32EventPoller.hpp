#pragma once

#include "../EventPoller.hpp"
#include <Windows.h>

namespace sovren::pal::win32 {

    class Win32EventPoller : public EventPoller {
    public:

        bool pollEvents() override;

        void waitEvents() override;

    private:
        MSG m_lastMsg = {};
    };

}
