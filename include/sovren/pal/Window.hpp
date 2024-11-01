#pragma once

#include "../Point2.hpp"
#include "../Size2.hpp"
#include "../pal/EventHandler.hpp"

#include "WindowHandle.hpp"

#include <memory>
#include <string>
#include <string_view>

namespace sovren {

    class Window {
    public:

        static auto create() -> std::unique_ptr<Window>;


        Window() = default;

        Window(const Window&) = delete;
        auto operator=(const Window&) -> Window& = delete;

        Window(Window&&) = delete;
        auto operator=(Window&&) -> Window& = delete;

        virtual ~Window() = default;


        virtual void setEventHandler(EventHandler* eventHandler) = 0;


        [[nodiscard]]
        virtual auto title() const -> std::string = 0;

        virtual void setTitle(std::string_view title) = 0;


        [[nodiscard]]
        virtual auto size() const -> Size2 = 0;

        virtual void setSize(Size2 size) = 0;


        [[nodiscard]]
        virtual auto isVisible() const -> bool = 0;

        virtual void setVisible(bool visible) = 0;


        [[nodiscard]]
        virtual auto mousePos() const -> Point2 = 0;

        [[nodiscard]]
        virtual auto handle() const -> WindowHandle = 0;
    };

}
