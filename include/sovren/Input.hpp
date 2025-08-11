#pragma once

#include <sovren/pal/Keys.hpp>
#include <sovren/pal/MouseButtons.hpp>

#include <sovren/util/Point2.hpp>

#include <algorithm>
#include <vector>

namespace sovren {

    class Input {
    public:
        [[nodiscard]]
        auto isDown(pal::Keys k) const noexcept -> bool {
            return std::ranges::find(pressedKeys_, k) != pressedKeys_.cend();
        }

        [[nodiscard]]
        auto isUp(pal::Keys k) const noexcept -> bool {
            return !isDown(k);
        }

        [[nodiscard]]
        auto isDown(pal::MouseButtons b) const noexcept -> bool {
            return std::ranges::find(pressedButtons_, b) != pressedButtons_.cend();
        }

        [[nodiscard]]
        auto isUp(pal::MouseButtons b) const noexcept -> bool {
            return !isDown(b);
        }

        [[nodiscard]]
        auto mousePos() const noexcept -> Point2i {
            return mousePos_;
        }

        void press(pal::Keys k) {
            pressedKeys_.push_back(k);
        }

        void release(pal::Keys k) {
            std::erase(pressedKeys_, k);
        }

        void press(pal::MouseButtons b) {
            pressedButtons_.push_back(b);
        }

        void release(pal::MouseButtons b) {
            std::erase(pressedButtons_, b);
        }

        void setMousePos(Point2i pos) {
            mousePos_ = pos;
        }

    private:
        std::vector<pal::MouseButtons> pressedButtons_;
        std::vector<pal::Keys> pressedKeys_;

        Point2i mousePos_;
    };

}
