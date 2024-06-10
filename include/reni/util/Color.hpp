#pragma once

#include "../math/Vec4.hpp"

#include <compare>

namespace reni {

    /**
     * @brief RGBA color in the form of four real components.
     */
    struct Color {

        friend std::partial_ordering operator<=>(Color lhs, Color rhs) noexcept = default;


        Color() noexcept = default;

        Color(Vec4 v) noexcept
            : Color(v.x, v.y, v.z, v.w) {}

        Color(float r, float g, float b, float a = 1.0) noexcept
            : r(r), g(g), b(b), a(a) {}


        operator Vec4() const noexcept {
            return { r, g, b, a };
        }


        float r = {};
        float g = {};
        float b = {};
        float a = {};
    };

}
