#pragma once

#include "../math/Mat4x4.hpp"
#include "../math/Vec3.hpp"

#include "SceneNode.hpp"

namespace sovren {

    /**
     * @brief Defines an affine transformation to apply to 3D geometry.
     */
    class Transform3D : public SceneNode {
    public:

        void accept(NodeVisitor& visitor) const override;


        /**
         * @brief Move the transform along the X axis.
         */
        void translateX(float dx) noexcept {
            translate({ dx, 0.0f, 0.0f });
        }


        /**
         * @brief Move the transform along the Y axis.
         */
        void translateY(float dy) noexcept {
            translate({ 0.0f, dy, 0.0f });
        }


        /**
         * @brief Move the transform along the Z axis.
         */
        void translateZ(float dz) noexcept {
            translate({ 0.0f, 0.0f, dz });
        }


        /**
         * @brief Move the transform along the XYZ axes.
         */
        void translate(Vec3 dr) noexcept {
            matrix_.r4.setXyz(matrix_.r4.xyz() + dr);
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


        Vec3 right() const noexcept {
            return matrix_.r1.xyz();
        }


        Vec3 up() const noexcept {
            return matrix_.r2.xyz();
        }


        Vec3 forward() const noexcept {
            return matrix_.r3.xyz();
        }


        Vec3 position() const noexcept {
            return matrix_.r4.xyz();
        }


        /**
         * @brief Present the transform in its matrix representation.
         */
        const Mat4x4& toMatrix() const noexcept {
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
