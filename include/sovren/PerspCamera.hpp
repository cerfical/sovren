#pragma once

#include "math/Mat4x4.hpp"
#include "math/util.hpp"

#include "Camera.hpp"

namespace sovren {

    class PerspCamera : public Camera {
    public:
        [[nodiscard]]
        auto projMatrix() const noexcept -> Mat4x4 override {
            return Mat4x4::perspective(aspectRatio(), fov_, nearPlane(), farPlane());
        }

        void setFov(float fov) noexcept {
            fov_ = fov;
        }

        [[nodiscard]]
        auto fov() const noexcept -> float {
            return fov_;
        }

    private:
        static constexpr float DefaultFov = degToRad(45);

        float fov_ = DefaultFov;
    };

}
