#pragma once

#include <sovren/sg/SceneNode3D.hpp>

#include <sovren/Camera.hpp>

#include <memory>

namespace sovren::sg {

    class CameraNode3D : public SceneNode3D {
    public:
        explicit CameraNode3D(std::unique_ptr<Camera> camera) noexcept
            : camera_(std::move(camera)) {}

        [[nodiscard]]
        auto camera() noexcept -> Camera* {
            return camera_.get();
        }

    private:
        std::unique_ptr<Camera> camera_;
    };

}
