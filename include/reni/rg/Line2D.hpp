#pragma once

#include "../math/Vec2.hpp"

#include "RenderNode.hpp"

namespace reni::rg {

    /**
     * @brief Describes a straight line in 2D space.
     */
    class Line2D : public RenderNode {
    public:

        /**
         * @brief Construct a new line from the start and end points.
         */
        Line2D(Vec2 start, Vec2 end) noexcept
            : start_(start), end_(end) {}


        void accept(NodeVisitor& visitor) const override;


        /**
         * @brief Starting point of the line.
         */
        Vec2 start() const noexcept {
            return start_;
        }


        /**
         * @brief Ending point of the line.
         */
        Vec2 end() const noexcept {
            return end_;
        }


    private:
        Vec2 start_;
        Vec2 end_;
    };

}
