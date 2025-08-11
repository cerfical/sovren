#pragma once

#include <sovren/log/Log.hpp>

#include <memory>

namespace sovren::pal {

    class EventPump {
    public:
        static auto create(log::Log = {}) -> std::unique_ptr<EventPump>;

        EventPump() = default;

        EventPump(const EventPump&) = delete;
        auto operator=(const EventPump&) -> EventPump& = delete;

        EventPump(EventPump&&) = delete;
        auto operator=(EventPump&&) -> EventPump& = delete;

        virtual ~EventPump() = default;

        virtual void pumpEvents() = 0;
    };

}
