#pragma once

#include "../Point2.hpp"
#include "../Size2.hpp"

#include "../input/Keys.hpp"
#include "../input/MouseButtons.hpp"

#include <memory>
#include <string>

namespace sovren {

    class Window {
    public:

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        Window(Window&&) noexcept;
        Window& operator=(Window&&) noexcept;

        Window();
        virtual ~Window();


        void setTitle(const std::string& newTitle);

        const std::string& title() const;


        void setSize(Size2 newSize);

        Size2 size() const;


        Point2 mousePos() const;

        bool keyState(Keys k) const;

        bool buttonState(MouseButtons b) const;

        void* nativeHandle() const;


        void show();


    private:
        virtual void onResize();

        virtual void onShow();
        virtual void onUpdate();
        virtual void onHide();

        virtual void onKeyDown(Keys k);
        virtual void onKeyUp(Keys k);

        virtual void onButtonDown(MouseButtons b);
        virtual void onButtonUp(MouseButtons b);
        virtual void onMouseMove();

        struct Impl;
        std::unique_ptr<Impl> impl_;
    };

}
