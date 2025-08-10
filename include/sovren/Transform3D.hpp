#pragma once

#include <sovren/math/Mat4x4.hpp>
#include <sovren/math/Vec3.hpp>

namespace sovren {

    class Transform3D {
    public:
        void translateX(float dx) noexcept {
            translate({ dx, 0.0f, 0.0f });
        }

        void translateY(float dy) noexcept {
            translate({ 0.0f, dy, 0.0f });
        }

        void translateZ(float dz) noexcept {
            translate({ 0.0f, 0.0f, dz });
        }

        void translate(Vec3 dr) noexcept {
            setPosition(position() + dr);
        }

        void setPosition(Vec3 pos) noexcept {
            matrix_.r4.setXyz(pos);
        }

        [[nodiscard]]
        auto position() const noexcept -> Vec3 {
            return matrix_.r4.xyz();
        }

        void strafe(float dx) noexcept {
            translate(right() * dx);
        }

        void fly(float dy) noexcept {
            translate(up() * dy);
        }

        void walk(float dz) noexcept {
            translate(forward() * dz);
        }

        void rotateX(float angle) noexcept {
            const auto r = Mat4x4::rotationX(angle);

            transformRight(r);
            transformUp(r);
            transformForward(r);
        }

        void rotateY(float angle) noexcept {
            const auto r = Mat4x4::rotationY(angle);

            transformRight(r);
            transformUp(r);
            transformForward(r);
        }

        void rotateZ(float angle) noexcept {
            const auto r = Mat4x4::rotationZ(angle);

            transformRight(r);
            transformUp(r);
            transformForward(r);
        }

        void rotateYaw(float angle) noexcept {
            const auto r = Mat4x4::rotationAxis(up(), angle);

            transformRight(r);
            transformForward(r);
        }

        void rotatePitch(float angle) noexcept {
            const auto r = Mat4x4::rotationAxis(right(), angle);

            transformUp(r);
            transformForward(r);
        }

        void rotateRoll(float angle) noexcept {
            const auto r = Mat4x4::rotationAxis(forward(), angle);

            transformRight(r);
            transformUp(r);
        }

        [[nodiscard]]
        auto right() const noexcept -> Vec3 {
            return matrix_.r1.xyz();
        }

        [[nodiscard]]
        auto up() const noexcept -> Vec3 {
            return matrix_.r2.xyz();
        }

        [[nodiscard]]
        auto forward() const noexcept -> Vec3 {
            return matrix_.r3.xyz();
        }

        [[nodiscard]]
        auto matrix() const noexcept -> const Mat4x4& {
            return matrix_;
        }

    private:
        void transformRight(const Mat4x4& mat) noexcept {
            matrix_.r1.setXyz(mat.transformNormal(right()));
        }

        void transformUp(const Mat4x4& mat) noexcept {
            matrix_.r2.setXyz(mat.transformNormal(up()));
        }

        void transformForward(const Mat4x4& mat) noexcept {
            matrix_.r3.setXyz(mat.transformNormal(forward()));
        }

        Mat4x4 matrix_ = Mat4x4::identity();
    };

}
