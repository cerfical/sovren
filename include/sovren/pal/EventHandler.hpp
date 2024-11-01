#pragma once

#include "Keys.hpp"
#include "MouseButtons.hpp"

namespace sovren {

    class EventHandler {
    public:

        EventHandler(const EventHandler&) = default;
        auto operator=(const EventHandler&) -> EventHandler& = default;

        EventHandler(EventHandler&&) = default;
        auto operator=(EventHandler&&) -> EventHandler& = default;


        virtual void onWindowClose() = 0;
        virtual void onWindowResize() = 0;

        virtual void onKeyDown(Keys k) = 0;
        virtual void onKeyUp(Keys k) = 0;

        virtual void onMouseButtonDown(MouseButtons b) = 0;
        virtual void onMouseButtonUp(MouseButtons b) = 0;
        virtual void onMouseMove() = 0;


    protected:
        EventHandler() = default;
        ~EventHandler() = default;
    };

}
