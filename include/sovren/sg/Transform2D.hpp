#pragma once

#include "../math/Mat3x3.hpp"
#include "../math/Vec2.hpp"

namespace sovren {

    /**
     * @brief Defines an affine transformation to apply to 2D geometry.
     */
    class Transform2D {
    public:

        /**
         * @brief Move the transform along the X axis.
         */
        void translateX(float dx) noexcept {
            translate({ dx, 0.0f });
        }


        /**
         * @brief Move the transform along the Y axis.
         */
        void translateY(float dy) noexcept {
            translate({ 0.0f, dy });
        }


        /**
         * @brief Move the transform along the XY axes.
         */
        void translate(Vec2 dr) noexcept {
            matrix_.r3.setXy(matrix_.r3.xy() + dr);
        }


        /**
         * @brief Present the transform in its matrix representation.
         */
        [[nodiscard]]
        auto toMatrix() const noexcept -> const Mat3x3& {
            return matrix_;
        }


    private:
        Mat3x3 matrix_ = Mat3x3::identity();
    };

}
