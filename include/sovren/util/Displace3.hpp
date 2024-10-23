#pragma once

#include "../math/Vec3.hpp"

#include <compare>

namespace sovren {

    /**
     * @brief Displacement of a point in 3D space as a triple of real numbers.
     */
    struct Displace3 {

        friend std::partial_ordering operator<=>(Displace3, Displace3) noexcept = default;


        Displace3() noexcept = default;

        Displace3(Vec3 vec) noexcept
            : Displace3(vec.x, vec.y, vec.z) {}

        Displace3(float dx, float dy, float dz) noexcept
            : dx(dx), dy(dy), dz(dz) {}


        operator Vec3() const noexcept {
            return { dx, dy, dz };
        }


        float dx = {};
        float dy = {};
        float dz = {};
    };

}
