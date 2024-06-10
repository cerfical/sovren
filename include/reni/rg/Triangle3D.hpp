#pragma once

#include "../util/Point3.hpp"

#include "SceneNode.hpp"

#include <array>
#include <span>

namespace reni {

    /**
     * @brief Describes a plain triangle in 3D space.
     */
    class Triangle3D : public SceneNode {
    public:

        /**
         * @brief Construct a new triangle from its points.
         */
        Triangle3D(Point3 p1, Point3 p2, Point3 p3) noexcept
            : points_{ p1, p2, p3 } {}


        void accept(NodeVisitor& visitor) const override;


        /**
         * @brief The points defining the triangle.
         */
        std::span<const Point3, 3> points() const noexcept {
            return points_;
        }


    private:
        std::array<Point3, 3> points_;
    };

}
