#pragma once

#include "../math/Vec2.hpp"

#include "NodeVisitor.hpp"
#include "SceneNode2D.hpp"

namespace sovren {

    /**
     * @brief Describes a straight line in 2D space.
     */
    class Line2D : public SceneNode2D {
    public:

        /**
         * @brief Construct a new line from the start and end points.
         */
        Line2D(Vec2 startPoint, Vec2 endPoint)
            : startPoint_(startPoint), endPoint_(endPoint) {}


        void acceptVisitor(NodeVisitor& visitor) const override {
            visitor.visit(*this);
        }


        /**
         * @brief Starting point of the line.
         */
        [[nodiscard]]
        auto startPoint() const noexcept -> Vec2 {
            return startPoint_;
        }


        /**
         * @brief Ending point of the line.
         */
        [[nodiscard]]
        auto endPoint() const noexcept -> Vec2 {
            return endPoint_;
        }


    private:
        Vec2 startPoint_;
        Vec2 endPoint_;
    };

}
