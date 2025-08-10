#pragma once

#include <sovren/math/Mat3x3.hpp>
#include <sovren/math/Vec2.hpp>

namespace sovren {

    class Transform2D {
    public:
        void translateX(float dx) noexcept {
            translate({ dx, 0.0f });
        }

        void translateY(float dy) noexcept {
            translate({ 0.0f, dy });
        }

        void translate(Vec2 dr) noexcept {
            setPosition(position() + dr);
        }

        void setPosition(Vec2 pos) noexcept {
            matrix_.r3.setXy(pos);
        }

        [[nodiscard]]
        auto position() const noexcept -> Vec2 {
            return matrix_.r3.xy();
        }

        [[nodiscard]]
        auto matrix() const noexcept -> const Mat3x3& {
            return matrix_;
        }

    private:
        Mat3x3 matrix_ = Mat3x3::identity();
    };

}
