#pragma once

#include "../math/Vec3.hpp"

#include "SceneNode.hpp"

#include <array>
#include <span>

namespace sovren {

    /**
     * @brief Describes a plain triangle in 3D space.
     */
    class Triangle3D : public SceneNode {
    public:

        /**
         * @brief Construct a new triangle from its points.
         */
        Triangle3D(Vec3 p1, Vec3 p2, Vec3 p3)
            : points_{ p1, p2, p3 } {}


        void accept(NodeVisitor& visitor) const override;


        /**
         * @brief The points defining the triangle.
         */
        [[nodiscard]]
        auto points() const noexcept -> std::span<const Vec3> {
            return points_;
        }


    private:
        std::array<Vec3, 3> points_;
    };

}
