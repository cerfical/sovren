#pragma once

#include <sovren/math/Mat3x3.hpp>
#include <sovren/math/Vec2.hpp>

namespace sovren {

    class Transform2D {
    public:
        Transform2D(const Mat3x3& m = Mat3x3::identity()) noexcept
            : matrix_(m) {}

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
        Mat3x3 matrix_;
    };

    inline auto operator*(const Transform2D& lhs, const Transform2D& rhs) noexcept -> Transform2D {
        return { lhs.matrix() * rhs.matrix() };
    }

    inline auto operator*=(Transform2D& lhs, const Transform2D& rhs) noexcept -> Transform2D& {
        lhs = lhs * rhs;
        return lhs;
    }

}
