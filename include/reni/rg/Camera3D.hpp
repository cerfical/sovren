#pragma once

#include "../math/Mat4x4.hpp"
#include "../math/util.hpp"

#include "../util/Size2.hpp"

#include "SceneNode.hpp"

#include <optional>

namespace reni {

    /**
     * @brief Presents 3D objects in a 2D plane using some projection matrix.
     */
    class Camera3D : public SceneNode {
    public:

        void accept(NodeVisitor& visitor) const override;


        void setNearPlane(float nearPlane) noexcept {
            nearPlane_ = nearPlane;
            updateProjMatrix();
        }

        float nearPlane() const noexcept {
            return nearPlane_;
        }


        void setFarPlane(float farPlane) noexcept {
            farPlane_ = farPlane;
            updateProjMatrix();
        }

        float farPlane() const noexcept {
            return farPlane_;
        }


        void setViewSize(Size2 viewSize) noexcept {
            viewSize_ = viewSize;
            updateProjMatrix();
        }

        Size2 viewSize() const noexcept {
            return viewSize_;
        }

        float aspectRatio() const noexcept {
            return viewSize_.width / viewSize_.height;
        }


        void setFov(float fov) noexcept {
            fov_ = fov;
            updateProjMatrix();
        }

        float fov() const noexcept {
            return fov_;
        }


        const Mat4x4& toProjMatrix() const noexcept {
            if(!projMatrix_) {
                projMatrix_ = buildProjMatrix();
            }
            return *projMatrix_;
        }


    private:
        virtual Mat4x4 buildProjMatrix() const noexcept = 0;

        void updateProjMatrix() noexcept {
            projMatrix_.reset();
        }


        static inline constexpr float FarPlaneDefault = 1000.0f;
        static inline constexpr float NearPlaneDefault = 1.0f;
        static inline constexpr float FovDefault = math::degToRad(45);


        mutable std::optional<Mat4x4> projMatrix_;

        Size2 viewSize_ = { 1.0f, 1.0f };
        float farPlane_ = FarPlaneDefault;
        float nearPlane_ = NearPlaneDefault;
        float fov_ = FovDefault;
    };

}
