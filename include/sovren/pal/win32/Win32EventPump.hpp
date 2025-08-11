#pragma once

#include <sovren/log/Log.hpp>

#include <sovren/pal/EventPump.hpp>

#include <sovren/pal/win32/errors.hpp>

#include <Windows.h>

namespace sovren::pal {

    class Win32EventPump : public EventPump {
    public:
        explicit Win32EventPump(log::Log log)
            : log_(log.withTags("EventPump")) {
            log_.info("Event pump created");
        }

        ~Win32EventPump() {
            log_.info("Event pump destroyed");
        }

        void pumpEvents() override {
            std::size_t eventCount = 0;
            while(PeekMessage(&lastMsg_, nullptr, 0, 0, PM_REMOVE) != 0) {
                TranslateMessage(&lastMsg_);
                DispatchMessage(&lastMsg_);

                ++eventCount;
            }

            if(eventCount > 0) {
                log_.info("Pumped {} event(s)", eventCount);
            }
        }

    private:
        log::Log log_;

        MSG lastMsg_ = {};
    };

}
