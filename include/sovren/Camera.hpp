#pragma once

#include "math/Mat4x4.hpp"
#include "util/Size2.hpp"

namespace sovren {

    class Camera {
    public:
        virtual ~Camera() = default;

        [[nodiscard]]
        virtual auto projMatrix() const noexcept -> Mat4x4 = 0;

        void setNearPlane(float nearPlane) noexcept {
            nearPlane_ = nearPlane;
        }

        [[nodiscard]]
        auto nearPlane() const noexcept -> float {
            return nearPlane_;
        }

        void setFarPlane(float farPlane) noexcept {
            farPlane_ = farPlane;
        }

        [[nodiscard]]
        auto farPlane() const noexcept -> float {
            return farPlane_;
        }

        void setViewSize(Size2f viewSize) noexcept {
            viewSize_ = viewSize;
        }

        [[nodiscard]]
        auto viewSize() const noexcept -> Size2f {
            return viewSize_;
        }

        [[nodiscard]]
        auto aspectRatio() const noexcept -> float {
            return viewSize_.width / viewSize_.height;
        }

    protected:
        Camera() = default;

        Camera(const Camera&) = default;
        auto operator=(const Camera&) -> Camera& = default;

        Camera(Camera&&) = default;
        auto operator=(Camera&&) -> Camera& = default;

    private:
        static constexpr float DefaultFarPlane = 1000;
        static constexpr float DefaultNearPlane = 1;
        static constexpr Size2f DefaultViewSize = { 1, 1 };

        Size2f viewSize_ = DefaultViewSize;
        float farPlane_ = DefaultFarPlane;
        float nearPlane_ = DefaultNearPlane;
    };

}
