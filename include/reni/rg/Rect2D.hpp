#pragma once

#include "../math/Vec2.hpp"

#include "RenderNode.hpp"

namespace reni::rg {

    /**
     * @brief Describes a rectangle in 2D space.
     */
    class Rect2D : public RenderNode {
    public:

        /**
         * @brief Construct a new rectangle from the top left and bottom right points.
         */
        Rect2D(Vec2 topLeft, Vec2 bottomRight) noexcept
            : topLeft_(topLeft), bottomRight_(bottomRight) {}


        void accept(NodeVisitor& visitor) const override;


        /**
         * @brief The point defining the top left corner of the rectangle.
         */
        Vec2 topLeft() const noexcept {
            return topLeft_;
        }


        /**
         * @brief The point defining the bottom right corner of the rectangle.
         */
        Vec2 bottomRight() const noexcept {
            return bottomRight_;
        }


    private:
        Vec2 topLeft_;
        Vec2 bottomRight_;
    };

}
