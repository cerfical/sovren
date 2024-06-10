#pragma once

namespace reni::pal {

    class EventPoller {
    public:

        EventPoller(const EventPoller&) = delete;
        EventPoller& operator=(const EventPoller&) = delete;

        EventPoller(EventPoller&&) = delete;
        EventPoller& operator=(EventPoller&&) = delete;

        EventPoller() = default;
        virtual ~EventPoller() = default;


        virtual bool pollEvents() = 0;

        virtual void waitEvents() = 0;
    };

}
