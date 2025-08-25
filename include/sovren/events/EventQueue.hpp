#pragma once

#include <sovren/events/Event.hpp>

#include <queue>

namespace sovren {

    class EventQueue {
    public:
        EventQueue() = default;

        EventQueue(const EventQueue&) = default;
        auto operator=(const EventQueue&) -> EventQueue& = default;

        EventQueue(EventQueue&&) = default;
        auto operator=(EventQueue&&) -> EventQueue& = default;

        ~EventQueue() = default;

        void push(const Event& e) {
            events_.push(e);
        }

        [[nodiscard]]
        auto pop() noexcept -> Event {
            auto e = events_.front();
            events_.pop();
            return e;
        }

        [[nodiscard]]
        auto isEmpty() const noexcept -> bool {
            return events_.empty();
        }

        [[nodiscard]]
        operator bool() const noexcept {
            return !isEmpty();
        }

    private:
        std::queue<Event> events_;
    };

}
