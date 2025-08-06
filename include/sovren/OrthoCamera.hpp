#pragma once

#include "math/Mat4x4.hpp"

#include "Camera.hpp"

namespace sovren {

    class OrthoCamera : public Camera {
    public:
        [[nodiscard]]
        auto projMatrix() const noexcept -> Mat4x4 override {
            return Mat4x4::orthographic(viewSize(), nearPlane(), farPlane());
        }
    };

}
