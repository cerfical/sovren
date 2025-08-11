#pragma once

#include <sovren/pal/WindowHandler.hpp>

#include <sovren/util/Handle.hpp>
#include <sovren/util/Point2.hpp>
#include <sovren/util/Size2.hpp>

#include <memory>
#include <string>
#include <string_view>

namespace sovren::pal {

    class Window {
    public:
        static auto create(WindowHandler* = {}) -> std::unique_ptr<Window>;

        Window() = default;

        Window(const Window&) = delete;
        auto operator=(const Window&) -> Window& = delete;

        Window(Window&&) = delete;
        auto operator=(Window&&) -> Window& = delete;

        virtual ~Window() = default;

        [[nodiscard]]
        virtual auto title() const -> std::string = 0;

        virtual void setTitle(std::string_view title) = 0;

        [[nodiscard]]
        virtual auto size() const -> Size2i = 0;

        virtual void setSize(Size2i size) = 0;

        [[nodiscard]]
        virtual auto isVisible() const -> bool = 0;

        virtual void setVisible(bool visible) = 0;

        [[nodiscard]]
        virtual auto mousePos() const -> Point2i = 0;

        [[nodiscard]]
        virtual auto handle() const -> Handle = 0;
    };

}
