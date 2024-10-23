#pragma once

#include "../core/Key.hpp"
#include "../core/MouseButton.hpp"

#include "../util/Point2.hpp"
#include "../util/Size2.hpp"

namespace sovren::pal {

    class WindowCallbacks {
    public:

        virtual void onWindowClose() = 0;

        virtual void onWindowResize(Size2 newSize) = 0;


        virtual void onKeyStateChange(Key key, bool pressed) = 0;


        virtual void onMouseButtonStateChange(MouseButton button, bool pressed) = 0;

        virtual void onMouseMove(Point2 newPos) = 0;

    protected:
        ~WindowCallbacks() = default;
    };

}
