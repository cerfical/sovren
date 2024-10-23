#include "core/Window.hpp"

#include "pal/Platform.hpp"
#include "pal/Window.hpp"
#include "pal/WindowCallbacks.hpp"

#include <algorithm>
#include <utility>
#include <vector>

namespace sovren {
    struct Window::Impl : public pal::WindowCallbacks {
        void onWindowClose() override {
            palWindow->setVisible(false);
            visible = false;
        }


        void onWindowResize(Size2 newSize) override {
            if(newSize != clientSize) {
                clientSize = newSize;
                window->onResize();
            }
        }


        void onKeyStateChange(Key k, bool pressed) override {
            const bool curKeyState = window->keyState(k);
            if(pressed) {
                if(!curKeyState) {
                    pressedKeys.push_back(k);
                    window->onKeyDown(k);
                }
            } else {
                if(curKeyState) {
                    std::erase(pressedKeys, k);
                    window->onKeyUp(k);
                }
            }
        }


        void onMouseButtonStateChange(MouseButton b, bool pressed) override {
            const bool curButtonState = window->buttonState(b);
            if(pressed) {
                if(!curButtonState) {
                    pressedButtons.push_back(b);
                    window->onButtonDown(b);
                }
            } else {
                if(curButtonState) {
                    std::erase(pressedButtons, b);
                    window->onButtonUp(b);
                }
            }
        }


        void onMouseMove(Point2 newPos) override {
            if(newPos != mousePos) {
                mousePos = newPos;
                window->onMouseMove();
            }
        }


        std::unique_ptr<pal::Window> palWindow;
        Window* window = {};

        std::string title;
        Size2 clientSize;
        bool visible = false;

        std::vector<MouseButton> pressedButtons;
        std::vector<Key> pressedKeys;
        Point2 mousePos;
    };


    Window::Window()
        : impl_(std::make_unique<Impl>()) {

        impl_->palWindow = pal::Platform::get()->createWindow();
        impl_->clientSize = impl_->palWindow->getClientSize();
        impl_->mousePos = impl_->palWindow->getMousePos();

        impl_->palWindow->installCallbacks(impl_.get());
        impl_->window = this;
    }


    void Window::setTitle(const std::string& newTitle) {
        impl_->palWindow->setTitle(newTitle);
        impl_->title = newTitle;
    }


    const std::string& Window::title() const {
        return impl_->title;
    }


    void Window::setSize(Size2 newSize) {
        impl_->palWindow->setClientSize(newSize);
    }


    Size2 Window::size() const {
        return impl_->clientSize;
    }


    Point2 Window::mousePos() const {
        return impl_->mousePos;
    }


    bool Window::keyState(Key k) const {
        return std::ranges::find(impl_->pressedKeys, k) != impl_->pressedKeys.cend();
    }


    bool Window::buttonState(MouseButton b) const {
        return std::ranges::find(impl_->pressedButtons, b) != impl_->pressedButtons.cend();
    }


    void* Window::nativeHandle() const {
        return impl_->palWindow->nativeHandle();
    }


    void Window::show() {
        onShow();

        impl_->palWindow->setVisible(true);
        impl_->visible = true;

        const auto eventPoller = pal::Platform::get()->createEventPoller();
        while(impl_->visible) {
            eventPoller->pollEvents();
            onUpdate();
        }

        onHide();
    }


    Window::Window(Window&& other) noexcept {
        *this = std::move(other);
    }

    Window& Window::operator=(Window&& other) noexcept {
        impl_ = std::move(other.impl_);
        impl_->window = this;
        return *this;
    }

    Window::~Window() = default;


    void Window::onResize() {}

    void Window::onShow() {}
    void Window::onUpdate() {}
    void Window::onHide() {}

    void Window::onKeyDown(Key) {}
    void Window::onKeyUp(Key) {}

    void Window::onButtonDown(MouseButton) {}
    void Window::onButtonUp(MouseButton) {}
    void Window::onMouseMove() {}
}
