#pragma once

#include "../math/Mat4x4.hpp"

#include "Camera3D.hpp"

namespace reni {

    /**
     * @brief Сreates a perspective projection of a 3D scene onto a 2D plane.
     */
    class PerspCamera3D : public Camera3D {
    private:
        Mat4x4 buildProjMatrix() const noexcept override {
            return Mat4x4::perspective(aspectRatio(), fov(), nearPlane(), farPlane());
        }
    };

}
