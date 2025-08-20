#pragma once

#include <sovren/math/Mat4x4.hpp>

#include <sovren/rhi/Buffer.hpp>
#include <sovren/rhi/Context3D.hpp>
#include <sovren/rhi/Device.hpp>
#include <sovren/rhi/Pipeline.hpp>

#include <sovren/Camera.hpp>
#include <sovren/Mesh.hpp>
#include <sovren/Transform3D.hpp>

#include <memory>
#include <vector>

namespace sovren {

    class RenderQueue3D {
    public:
        explicit RenderQueue3D(std::shared_ptr<rhi::Device>);

        RenderQueue3D(const RenderQueue3D&) = delete;
        auto operator=(const RenderQueue3D&) -> RenderQueue3D& = delete;

        RenderQueue3D(RenderQueue3D&&) = default;
        auto operator=(RenderQueue3D&&) -> RenderQueue3D& = default;

        ~RenderQueue3D() = default;

        struct MeshItem {
            const Mesh* mesh = {};
            Transform3D transform;
        };

        void drawMesh(const MeshItem& m) {
            meshItems_.push_back(m);
        }

        struct CameraItem {
            const Camera* camera = {};
            Transform3D transform;
        };

        void setCamera(const CameraItem& c) {
            camera_ = c;
        }

        void flush(rhi::Context3D& ctx) {
            const auto viewProj = makeViewProjMatrix();
            for(const auto& item : meshItems_) {
                if(!item.mesh) {
                    // no mesh to render
                    continue;
                }

                // TODO: Reuse buffers across frames
                const auto buffer = device_->createBuffer({
                    .data = std::as_bytes(item.mesh->vertices()),
                    .gpuAccess = rhi::AccessModes::Read,
                    .bind = rhi::BindModes::VertexBuffer,
                });

                ctx.setPipeline(pipeline_.get());
                ctx.setVertexBuffer(buffer.get(), 12);
                ctx.setConstantBuffer(objectBuffer_.get(), rhi::ShaderStages::Vertex);

                writeTransform(ctx, item.transform.matrix() * viewProj);
                ctx.draw(item.mesh->size());
            }
            meshItems_.clear();
        }

    private:
        [[nodiscard]]
        auto makeViewProjMatrix() const noexcept -> Mat4x4 {
            if(camera_.camera) {
                const auto viewMatrix = camera_.transform.inverse().matrix();
                return viewMatrix * camera_.camera->projMatrix();
            }
            return Mat4x4::identity();
        }

        void writeTransform(rhi::Context3D& ctx, const Mat4x4& m) {
            auto buf = ctx.map(objectBuffer_.get(), rhi::AccessModes::Write);
            *reinterpret_cast<Mat4x4*>(buf.data()) = m;
            ctx.unmap(objectBuffer_.get());
        }

        std::shared_ptr<rhi::Device> device_;

        std::unique_ptr<rhi::Pipeline> pipeline_;
        std::unique_ptr<rhi::Buffer> objectBuffer_;

        std::vector<MeshItem> meshItems_;
        CameraItem camera_;
    };

}
