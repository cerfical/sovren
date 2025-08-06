#pragma once

#include "math/Mat4x4.hpp"
#include "rhi/Context3D.hpp"

#include "CameraNode3D.hpp"
#include "RenderNode.hpp"
#include "SceneNode.hpp"
#include "SceneNode3D.hpp"

#include <memory>
#include <stack>

namespace sovren {

    class Renderer3D {
    public:
        Renderer3D() {
            // the root transform is always an identity matrix
            transforms_.push(Mat4x4::identity());
            viewProj_ = Mat4x4::identity();
        }

        void renderScene(RenderNode& scene) {
            if(!objectBuffer_) {
                objectBuffer_ = scene.device()->createBuffer({
                    .size = sizeof(Mat4x4),
                    .cpuAccess = rhi::AccessModes::Write,
                    .gpuAccess = rhi::AccessModes::Read,
                    .bind = rhi::BindModes::ConstantBuffer,
                });
            }
            auto& ctx = *scene.context3D();
            ctx.setRenderTarget(scene.target());
            ctx.setConstantBuffer(objectBuffer_.get(), rhi::ShaderStages::Vertex);
            ctx.setViewport({ { 0, 0 }, static_cast<Size2f>(scene.window()->size()) });

            renderNode(ctx, scene);
        }

    private:
        void renderNode(rhi::Context3D& ctx, SceneNode& n) {
            if(auto* const node3D = dynamic_cast<SceneNode3D*>(&n)) {
                renderNode3D(ctx, *node3D);
            } else {
                renderChildren(ctx, n);
            }
        }

        void renderNode3D(rhi::Context3D& ctx, SceneNode3D& n) {
            transforms_.push(transforms_.top() * n.transform().matrix());

            writeTransform(ctx, transforms_.top() * viewProj_);
            n.render();

            if(auto* const cameraNode = dynamic_cast<CameraNode3D*>(&n)) {
                // position child nodes relative to the camera
                const auto prevViewProj = viewProj_;
                viewProj_ = invert(transforms_.top()) * cameraNode->camera()->projMatrix();
                transforms_.push(Mat4x4::identity());

                renderChildren(ctx, n);

                transforms_.pop();
                viewProj_ = prevViewProj;
            } else {
                renderChildren(ctx, n);
            }

            transforms_.pop();
        }

        void renderChildren(rhi::Context3D& ctx, SceneNode& n) {
            for(const auto& child : n.children()) {
                renderNode(ctx, *child);
            }
        }

        void writeTransform(rhi::Context3D& ctx, const Mat4x4& m) {
            auto buf = ctx.map(objectBuffer_.get(), rhi::AccessModes::Write);
            *reinterpret_cast<Mat4x4*>(buf.data()) = m;
            ctx.unmap(objectBuffer_.get());
        }

        std::unique_ptr<rhi::Buffer> objectBuffer_;

        std::stack<Mat4x4> transforms_;
        Mat4x4 viewProj_;
    };

}
