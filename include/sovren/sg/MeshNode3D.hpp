#pragma once

#include <sovren/sg/SceneNode3D.hpp>

#include <sovren/Mesh.hpp>
#include <sovren/RenderList.hpp>

#include <utility>

namespace sovren::sg {

    class MeshNode3D : public SceneNode3D {
    public:
        explicit MeshNode3D(Mesh mesh) noexcept
            : mesh_(std::move(mesh)) {}

        void init() override {
            server()->registerMesh(&mesh_);
            SceneNode3D::init();
        }

        void free() override {
            SceneNode3D::free();
            server()->unregisterMesh(&mesh_);
        }

        void render(RenderList& list) override {
            list.addItem3D({
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
