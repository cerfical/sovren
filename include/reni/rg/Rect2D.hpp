#pragma once

#include "../util/Point2.hpp"

#include "SceneNode.hpp"

namespace reni {

    /**
     * @brief Describes a rectangle in 2D space.
     */
    class Rect2D : public SceneNode {
    public:

        /**
         * @brief Construct a new rectangle from the top left and bottom right points.
         */
        Rect2D(Point2 topLeft, Point2 bottomRight) noexcept
            : topLeft_(topLeft), bottomRight_(bottomRight) {}


        void accept(NodeVisitor& visitor) const override;


        /**
         * @brief The point defining the top left corner of the rectangle.
         */
        Point2 topLeft() const noexcept {
            return topLeft_;
        }


        /**
         * @brief The point defining the bottom right corner of the rectangle.
         */
        Point2 bottomRight() const noexcept {
            return bottomRight_;
        }


    private:
        Point2 topLeft_;
        Point2 bottomRight_;
    };

}
