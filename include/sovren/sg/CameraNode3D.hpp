#pragma once

#include <sovren/sg/SceneNode3D.hpp>

#include <sovren/Camera.hpp>
#include <sovren/RenderList.hpp>

#include <memory>

namespace sovren::sg {

    class CameraNode3D : public SceneNode3D {
    public:
        explicit CameraNode3D(std::unique_ptr<Camera> camera) noexcept
            : camera_(std::move(camera)) {}

        void render(RenderList& list) override {
            const auto viewMatrix = worldTransform().inverse().matrix();
            const auto& viewProjMatrix = viewMatrix * camera_->projMatrix();

            list.setCameraMatrix(viewProjMatrix);
        }

        [[nodiscard]]
        auto camera() const noexcept -> const Camera& {
            return *camera_;
        }

        [[nodiscard]]
        auto camera() noexcept -> Camera& {
            return *camera_;
        }

    private:
        std::unique_ptr<Camera> camera_;
    };

}
