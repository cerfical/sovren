#pragma once

#include "../util/Point2.hpp"
#include "../util/Size2.hpp"

#include "Key.hpp"
#include "MouseButton.hpp"

#include <memory>
#include <string>

namespace reni {

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

        bool keyState(Key k) const;

        bool buttonState(MouseButton b) const;

        void* nativeHandle() const;


        void show();


    private:
        virtual void onResize();

        virtual void onShow();
        virtual void onUpdate();
        virtual void onHide();

        virtual void onKeyDown(Key key);
        virtual void onKeyUp(Key key);

        virtual void onButtonDown(MouseButton button);
        virtual void onButtonUp(MouseButton button);
        virtual void onMouseMove();

        struct Impl;
        std::unique_ptr<Impl> impl_;
    };

}
