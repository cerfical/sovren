#pragma once

#include <sovren/pal/Key.hpp>
#include <sovren/pal/MouseButton.hpp>

#include <sovren/util/Point2.hpp>
#include <sovren/util/Size2.hpp>

#include <sovren/events/KeyEvent.hpp>
#include <sovren/events/KeyEventType.hpp>
#include <sovren/events/MouseEvent.hpp>
#include <sovren/events/MouseEventType.hpp>
#include <sovren/events/WindowEvent.hpp>
#include <sovren/events/WindowEventType.hpp>

#include <utility>
#include <variant>

namespace sovren {

    class Event {
    public:
        Event() = default;

        Event(WindowEventType type, Size2i size = {}) noexcept
            : event_(std::in_place_type<WindowEvent>, size, type) {}

        Event(KeyEventType type, pal::Key key = {}) noexcept
            : event_(std::in_place_type<KeyEvent>, key, type) {}

        Event(MouseEventType type, pal::MouseButton button = {}, Point2i pos = {}) noexcept
            : event_(std::in_place_type<MouseEvent>, pos, button, type) {}

        Event(const Event&) = default;
        auto operator=(const Event&) -> Event& = default;

        Event(Event&&) = default;
        auto operator=(Event&&) -> Event& = default;

        ~Event() = default;

        template <typename T>
        [[nodiscard]]
        auto as() const noexcept -> const T* {
            return std::get_if<T>(&event_);
        }

        template <typename T>
        [[nodiscard]]
        auto is() const noexcept -> bool {
            return as<T>() != nullptr;
        }

        [[nodiscard]]
        auto isEmpty() const noexcept -> bool {
            return !is<std::monostate>();
        }

        [[nodiscard]]
        operator bool() const noexcept {
            return !isEmpty();
        }

    private:
        std::variant<std::monostate, WindowEvent, MouseEvent, KeyEvent> event_;
    };

}
