#pragma once

#include "../math/Mat4x4.hpp"
#include "../math/util.hpp"

#include "../Size2.hpp"

#include "NodeVisitor.hpp"
#include "SceneNode.hpp"

#include <optional>

namespace sovren {

    /**
     * @brief Presents 3D objects in a 2D plane using some projection matrix.
     */
    class Camera3D : public SceneNode {
    public:

        void accept(NodeVisitor& visitor) const override {
            visitor.visit(*this);
        }


        void setNearPlane(float nearPlane) noexcept {
            nearPlane_ = nearPlane;
            updateProjMatrix();
        }

        [[nodiscard]]
        auto nearPlane() const noexcept -> float {
            return nearPlane_;
        }


        void setFarPlane(float farPlane) noexcept {
            farPlane_ = farPlane;
            updateProjMatrix();
        }

        [[nodiscard]]
        auto farPlane() const noexcept -> float {
            return farPlane_;
        }


        void setViewSize(Size2 viewSize) noexcept {
            viewSize_ = viewSize;
            updateProjMatrix();
        }

        [[nodiscard]]
        auto viewSize() const noexcept -> Size2 {
            return viewSize_;
        }

        [[nodiscard]]
        auto aspectRatio() const noexcept -> float {
            return static_cast<float>(viewSize_.width) / static_cast<float>(viewSize_.height);
        }


        void setFov(float fov) noexcept {
            fov_ = fov;
            updateProjMatrix();
        }

        [[nodiscard]]
        auto fov() const noexcept -> float {
            return fov_;
        }


        [[nodiscard]]
        auto toProjMatrix() const noexcept -> const Mat4x4& {
            if(!projMatrix_) {
                projMatrix_ = buildProjMatrix();
            }
            return *projMatrix_;
        }


    private:

        [[nodiscard]]
        virtual auto buildProjMatrix() const noexcept -> Mat4x4 = 0;

        void updateProjMatrix() noexcept {
            projMatrix_.reset();
        }


        static constexpr float FarPlaneDefault = 1000;
        static constexpr float NearPlaneDefault = 1;
        static constexpr float FovDefault = degToRad(45);


        mutable std::optional<Mat4x4> projMatrix_;

        Size2 viewSize_ = { .width = 1, .height = 1 };
        float farPlane_ = FarPlaneDefault;
        float nearPlane_ = NearPlaneDefault;
        float fov_ = FovDefault;
    };

}
