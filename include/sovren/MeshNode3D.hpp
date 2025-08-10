#pragma once

#include "rhi/Buffer.hpp"
#include "rhi/Pipeline.hpp"

#include "Mesh.hpp"
#include "SceneNode3D.hpp"

#include <memory>
#include <utility>

namespace sovren {

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
