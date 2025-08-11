#pragma once

#include <sovren/pal/EventPump.hpp>

#include <sovren/pal/win32/errors.hpp>

#include <Windows.h>

namespace sovren::pal {

    class Win32EventPump : public EventPump {
    public:
        void pumpEvents() override {
            while(PeekMessage(&lastMsg_, nullptr, 0, 0, PM_REMOVE) != 0) {
                TranslateMessage(&lastMsg_);
                DispatchMessage(&lastMsg_);
            }
        }

    private:
        MSG lastMsg_ = {};
    };

}
