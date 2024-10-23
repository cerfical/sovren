#pragma once

#include "../util/Point2.hpp"

#include "SceneNode.hpp"

namespace sovren {

    /**
     * @brief Describes a straight line in 2D space.
     */
    class Line2D : public SceneNode {
    public:

        /**
         * @brief Construct a new line from the start and end points.
         */
        Line2D(Point2 start, Point2 end) noexcept
            : start_(start), end_(end) {}


        void accept(NodeVisitor& visitor) const override;


        /**
         * @brief Starting point of the line.
         */
        Point2 start() const noexcept {
            return start_;
        }


        /**
         * @brief Ending point of the line.
         */
        Point2 end() const noexcept {
            return end_;
        }


    private:
        Point2 start_;
        Point2 end_;
    };

}
