#pragma once

#include "../EventPump.hpp"
#include "error_util.hpp"

#include <Windows.h>

namespace sovren::win32 {

    class Win32EventPump : public EventPump {
    public:

        void pumpEvents() override {
            while(PeekMessage(&lastMsg_, nullptr, 0, 0, PM_REMOVE) != 0) {
                TranslateMessage(&lastMsg_);
                DispatchMessage(&lastMsg_);
            }
        }

        void waitEvents() override {
            win32Check(WaitMessage());
        }

    private:
        MSG lastMsg_ = {};
    };

}
