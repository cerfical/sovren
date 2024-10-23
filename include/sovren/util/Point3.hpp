#pragma once

#include "../math/Vec3.hpp"

#include <compare>

namespace sovren {

    /**
     * @brief Coordinates of a point in 3D space as a triple of real numbers.
     */
    struct Point3 {

        friend std::partial_ordering operator<=>(Point3, Point3) noexcept = default;


        Point3() noexcept = default;

        Point3(Vec3 vec) noexcept
            : Point3(vec.x, vec.y, vec.z) {}

        Point3(float x, float y, float z) noexcept
            : x(x), y(y), z(z) {}


        operator Vec3() const noexcept {
            return { x, y, z };
        }


        float x = {};
        float y = {};
        float z = {};
    };

}
