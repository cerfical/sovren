#pragma once

#include <sovren/sg/SceneNode3D.hpp>

#include <sovren/Mesh.hpp>
#include <sovren/RenderQueue3D.hpp>

#include <utility>

namespace sovren::sg {

    class MeshNode3D : public SceneNode3D {
    public:
        explicit MeshNode3D(Mesh mesh) noexcept
            : mesh_(std::move(mesh)) {}

        void render(RenderQueue3D& queue) const override {
            queue.drawMesh({
                .mesh = &mesh_,
                .transform = worldTransform(),
            });
        }

        [[nodiscard]]
        auto mesh() const noexcept -> const Mesh& {
            return mesh_;
        }

        [[nodiscard]]
        auto mesh() noexcept -> Mesh& {
            return mesh_;
        }

    private:
        Mesh mesh_;
    };

}
