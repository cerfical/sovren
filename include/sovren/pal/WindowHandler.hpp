#pragma once

#include <sovren/pal/Keys.hpp>
#include <sovren/pal/MouseButtons.hpp>

namespace sovren {

    class WindowHandler {
    public:
        virtual void windowClose() = 0;
        virtual void windowResize() = 0;

        virtual void keyDown(Keys) = 0;
        virtual void keyUp(Keys) = 0;

        virtual void mouseButtonDown(MouseButtons) = 0;
        virtual void mouseButtonUp(MouseButtons) = 0;
        virtual void mouseMove() = 0;

    protected:
        ~WindowHandler() = default;
    };

}
