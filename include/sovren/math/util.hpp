#pragma once

#include <numbers>

namespace sovren {

    /**
     * @brief Mathematical constant Pi.
     */
    inline constexpr float Pi = std::numbers::pi_v<float>;


    /**
     * @brief The number of degrees in one radian.
     */
    inline constexpr float RadToDeg = 180.0f / Pi;


    /**
     * @brief The number of radians in one degree.
     */
    inline constexpr float DegToRad = 1.0f / RadToDeg;


    /**
     * @brief Convert angle units from degrees to radians.
     */
    constexpr auto degToRad(float deg) noexcept -> float {
        return deg * DegToRad;
    }


    /**
     * @brief Convert angle units from radians to degrees.
     */
    constexpr auto radToDeg(float rad) noexcept -> float {
        return rad * RadToDeg;
    }

}
