#pragma once

#include <sovren/pal/Key.hpp>
#include <sovren/pal/MouseButton.hpp>

#include <sovren/util/Point2.hpp>

#include <algorithm>
#include <vector>

namespace sovren {

    class Input {
    public:
        [[nodiscard]]
        auto isDown(pal::Key k) const noexcept -> bool {
            return std::ranges::find(pressedKeys_, k) != pressedKeys_.cend();
        }

        [[nodiscard]]
        auto isUp(pal::Key k) const noexcept -> bool {
            return !isDown(k);
        }

        [[nodiscard]]
        auto isDown(pal::MouseButton b) const noexcept -> bool {
            return std::ranges::find(pressedButtons_, b) != pressedButtons_.cend();
        }

        [[nodiscard]]
        auto isUp(pal::MouseButton b) const noexcept -> bool {
            return !isDown(b);
        }

        [[nodiscard]]
        auto mousePos() const noexcept -> Point2i {
            return mousePos_;
        }

        void press(pal::Key k) {
            pressedKeys_.push_back(k);
        }

        void release(pal::Key k) {
            std::erase(pressedKeys_, k);
        }

        void press(pal::MouseButton b) {
            pressedButtons_.push_back(b);
        }

        void release(pal::MouseButton b) {
            std::erase(pressedButtons_, b);
        }

        void setMousePos(Point2i pos) {
            mousePos_ = pos;
        }

    private:
        std::vector<pal::MouseButton> pressedButtons_;
        std::vector<pal::Key> pressedKeys_;

        Point2i mousePos_;
    };

}
