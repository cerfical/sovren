#pragma once

#include <sovren/pal/Key.hpp>
#include <sovren/pal/MouseButton.hpp>

namespace sovren::pal {

    class WindowHandler {
    public:
        virtual void windowClose() = 0;
        virtual void windowResize() = 0;

        virtual void keyDown(Key) = 0;
        virtual void keyUp(Key) = 0;

        virtual void mouseButtonDown(MouseButton) = 0;
        virtual void mouseButtonUp(MouseButton) = 0;
        virtual void mouseMove() = 0;

    protected:
        ~WindowHandler() = default;
    };

}
