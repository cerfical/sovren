#pragma once

#include <sovren/math/Mat4x4.hpp>

#include <sovren/Camera.hpp>

namespace sovren {

    class OrthoCamera : public Camera {
    public:
        [[nodiscard]]
        auto projMatrix() const noexcept -> Mat4x4 override {
            return Mat4x4::orthographic(viewSize(), nearPlane(), farPlane());
        }
    };

}
