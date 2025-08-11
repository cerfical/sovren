#pragma once

#include <sovren/rhi/Buffer.hpp>
#include <sovren/rhi/Pipeline.hpp>

#include <sovren/sg/SceneNode3D.hpp>

#include <sovren/Mesh.hpp>

#include <memory>
#include <utility>

namespace sovren::sg {

    class MeshNode3D : public SceneNode3D {
    public:
        explicit MeshNode3D(Mesh mesh) noexcept
            : mesh_(std::move(mesh)) {}

        void init() override;

        void free() override {
            SceneNode3D::free();

            buffer_.reset();
            pipeline_.reset();
        }

        void render() override {
            context3D()->setPipeline(pipeline_.get());
            context3D()->setVertexBuffer(buffer_.get(), 12);
            context3D()->draw(mesh_.vertices().size());
        }

    private:
        std::unique_ptr<rhi::Buffer> buffer_;
        std::unique_ptr<rhi::Pipeline> pipeline_;

        Mesh mesh_;
    };

}
