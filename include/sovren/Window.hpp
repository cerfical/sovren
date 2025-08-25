#pragma once

#include <sovren/events/EventQueue.hpp>

#include <sovren/util/Color.hpp>
#include <sovren/util/Size2.hpp>

#include <string>

namespace sovren {

    class Window {
    public:
        Window() = default;

        Window(const Window&) = default;
        auto operator=(const Window&) -> Window& = default;

        Window(Window&&) = default;
        auto operator=(Window&&) -> Window& = default;

        ~Window() = default;

        void setTitle(std::string title) noexcept {
            title_ = std::move(title);
        }

        [[nodiscard]]
        auto title() const noexcept -> const std::string& {
            return title_;
        }

        void setSize(Size2i size) noexcept {
            size_ = size;
        }

        [[nodiscard]]
        auto size() const noexcept -> Size2i {
            return size_;
        }

        [[nodiscard]]
        auto width() const noexcept -> int {
            return size().width;
        }

        [[nodiscard]]
        auto height() const noexcept -> int {
            return size().height;
        }

        void setVisible(bool visible) noexcept {
            visible_ = visible;
        }

        void show() noexcept {
            setVisible(true);
        }

        void hide() noexcept {
            setVisible(false);
        }

        [[nodiscard]]
        auto isVisible() const noexcept -> bool {
            return visible_;
        }

        void setBackgroundColor(Color c) noexcept {
            bgColor_ = c;
        }

        [[nodiscard]]
        auto backgroundColor() const noexcept -> Color {
            return bgColor_;
        }

        [[nodiscard]]
        auto events() noexcept -> EventQueue& {
            return events_;
        }

    private:
        static constexpr auto DefaultSize = Size2i(1024, 640);
        static constexpr auto DefaultBgColor = Color::white();

        std::string title_;
        Color bgColor_ = DefaultBgColor;
        Size2i size_ = DefaultSize;
        bool visible_ = {};

        EventQueue events_;
    };

}
