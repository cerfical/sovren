#pragma once

#include "pal/Keys.hpp"
#include "pal/MouseButtons.hpp"
#include "util/Point2.hpp"

#include <algorithm>
#include <vector>

namespace sovren {

    class Input {
    public:
        [[nodiscard]]
        auto isDown(Keys k) const noexcept -> bool {
            return std::ranges::find(pressedKeys_, k) != pressedKeys_.cend();
        }

        [[nodiscard]]
        auto isUp(Keys k) const noexcept -> bool {
            return !isDown(k);
        }

        [[nodiscard]]
        auto isDown(MouseButtons b) const noexcept -> bool {
            return std::ranges::find(pressedButtons_, b) != pressedButtons_.cend();
        }

        [[nodiscard]]
        auto isUp(MouseButtons b) const noexcept -> bool {
            return !isDown(b);
        }

        [[nodiscard]]
        auto mousePos() const noexcept -> Point2i {
            return mousePos_;
        }

        void press(Keys k) {
            pressedKeys_.push_back(k);
        }

        void release(Keys k) {
            std::erase(pressedKeys_, k);
        }

        void press(MouseButtons b) {
            pressedButtons_.push_back(b);
        }

        void release(MouseButtons b) {
            std::erase(pressedButtons_, b);
        }

        void setMousePos(Point2i pos) {
            mousePos_ = pos;
        }

    private:
        std::vector<MouseButtons> pressedButtons_;
        std::vector<Keys> pressedKeys_;

        Point2i mousePos_;
    };

}
