#pragma once

#include <memory>

namespace sovren {

    class EventPump {
    public:

        static auto create() -> std::unique_ptr<EventPump>;


        EventPump() = default;

        EventPump(const EventPump&) = delete;
        auto operator=(const EventPump&) -> EventPump& = delete;

        EventPump(EventPump&&) = delete;
        auto operator=(EventPump&&) -> EventPump& = delete;

        virtual ~EventPump() = default;


        virtual void pumpEvents() = 0;

        virtual void waitEvents() = 0;
    };

}
