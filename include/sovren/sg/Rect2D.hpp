#pragma once

#include "../math/Vec2.hpp"

#include "NodeVisitor.hpp"
#include "SceneNode.hpp"

namespace sovren {

    /**
     * @brief Describes a rectangle in 2D space.
     */
    class Rect2D : public SceneNode {
    public:

        /**
         * @brief Construct a new rectangle from the top left and bottom right points.
         */
        Rect2D(Vec2 topLeft, Vec2 bottomRight)
            : topLeft_(topLeft), bottomRight_(bottomRight) {}


        void acceptVisitor(NodeVisitor& visitor) const override {
            visitor.visit(*this);
        }


        /**
         * @brief The point defining the top left corner of the rectangle.
         */
        [[nodiscard]]
        auto topLeftPoint() const noexcept -> Vec2 {
            return topLeft_;
        }


        /**
         * @brief The point defining the bottom right corner of the rectangle.
         */
        [[nodiscard]]
        auto bottomRightPoint() const noexcept -> Vec2 {
            return bottomRight_;
        }


    private:
        Vec2 topLeft_;
        Vec2 bottomRight_;
    };

}
