#pragma once

#include <sovren/math/Mat4x4.hpp>

#include <sovren/util/Color.hpp>

#include <sovren/Mesh.hpp>
#include <sovren/Shape.hpp>
#include <sovren/Transform2D.hpp>
#include <sovren/Transform3D.hpp>
#include <sovren/Window.hpp>

#include <span>
#include <vector>

namespace sovren {

    struct RenderItem2D {
        Shape shape;
        Color outlineColor;
        Color fillColor;
        Transform2D transform;
    };

    struct RenderItem3D {
        const Mesh* mesh = {};
        Transform3D transform;
    };

    class RenderList {
    public:
        RenderList() = default;

        RenderList(const RenderList&) = default;
        auto operator=(const RenderList&) -> RenderList& = default;

        RenderList(RenderList&&) = default;
        auto operator=(RenderList&&) -> RenderList& = default;

        ~RenderList() = default;

        void setTargetWindow(Window* wnd) noexcept {
            window_ = wnd;
        }

        [[nodiscard]]
        auto targetWindow() const noexcept -> Window* {
            return window_;
        }

        void setCameraMatrix(const Mat4x4& m) noexcept {
            cameraMatrix_ = m;
        }

        [[nodiscard]]
        auto cameraMatrix() const noexcept -> const Mat4x4& {
            return cameraMatrix_;
        }

        void addItem3D(const RenderItem3D& item) {
            items3D_.push_back(item);
        }

        [[nodiscard]]
        auto items3D() const noexcept -> std::span<const RenderItem3D> {
            return items3D_;
        }

        void addItem2D(const RenderItem2D& item) {
            items2D_.push_back(item);
        }

        [[nodiscard]]
        auto items2D() const noexcept -> std::span<const RenderItem2D> {
            return items2D_;
        }

        void reset() noexcept {
            items3D_.clear();
            items2D_.clear();

            cameraMatrix_ = Mat4x4::identity();
            window_ = {};
        }

    private:
        std::vector<RenderItem3D> items3D_;
        std::vector<RenderItem2D> items2D_;

        Mat4x4 cameraMatrix_ = Mat4x4::identity();
        Window* window_ = {};
    };

}
