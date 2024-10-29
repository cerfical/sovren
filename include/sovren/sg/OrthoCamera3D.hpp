#pragma once

#include "../math/Mat4x4.hpp"

#include "Camera3D.hpp"

namespace sovren {

    /**
     * @brief Сreates an orthographic projection of a 3D scene onto a 2D plane.
     */
    class OrthoCamera3D : public Camera3D {
    private:
        [[nodiscard]]
        auto buildProjMatrix() const noexcept -> Mat4x4 override {
            return Mat4x4::orthographic(viewSize(), nearPlane(), farPlane());
        }
    };

}
