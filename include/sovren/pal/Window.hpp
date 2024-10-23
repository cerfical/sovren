#pragma once

#include "../Point2.hpp"
#include "../Size2.hpp"

#include <string>
#include <string_view>

namespace sovren::pal {

    class WindowCallbacks;


    class Window {
    public:

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        Window(Window&&) = delete;
        Window& operator=(Window&&) = delete;

        Window() = default;
        virtual ~Window() = default;


        virtual void installCallbacks(WindowCallbacks* callbacks) = 0;


        virtual void setTitle(std::string_view newTitle) = 0;

        virtual void setClientSize(Size2 newSize) = 0;

        virtual void setVisible(bool visible) = 0;


        virtual std::string getTitle() const = 0;

        virtual Size2 getClientSize() const = 0;

        virtual bool isVisible() const = 0;


        virtual Point2 getMousePos() const = 0;

        virtual void* nativeHandle() const = 0;
    };

}
