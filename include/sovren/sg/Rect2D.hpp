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
        Rect2D(Vec2 topLeftPoint, Vec2 botRightPoint)
            : topLeftPoint_(topLeftPoint), botRightPoint_(botRightPoint) {}


        void accept(NodeVisitor& visitor) const override {
            visitor.visit(*this);
        }


        /**
         * @brief The point defining the top left corner of the rectangle.
         */
        [[nodiscard]]
        auto topLeftPoint() const noexcept -> Vec2 {
            return topLeftPoint_;
        }


        /**
         * @brief The point defining the bottom right corner of the rectangle.
         */
        [[nodiscard]]
        auto botRightPoint() const noexcept -> Vec2 {
            return botRightPoint_;
        }


    private:
        Vec2 topLeftPoint_;
        Vec2 botRightPoint_;
    };

}
