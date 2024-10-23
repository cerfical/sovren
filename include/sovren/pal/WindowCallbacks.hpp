#pragma once

#include "../input/Keys.hpp"
#include "../input/MouseButtons.hpp"

#include "../Point2.hpp"
#include "../Size2.hpp"

namespace sovren::pal {

    class WindowCallbacks {
    public:

        virtual void onWindowClose() = 0;

        virtual void onWindowResize(Size2 newSize) = 0;


        virtual void onKeyStateChange(Keys key, bool pressed) = 0;


        virtual void onMouseButtonsStateChange(MouseButtons button, bool pressed) = 0;

        virtual void onMouseMove(Point2 newPos) = 0;

    protected:
        ~WindowCallbacks() = default;
    };

}
