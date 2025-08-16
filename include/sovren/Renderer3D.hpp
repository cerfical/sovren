#pragma once

#include <sovren/math/Mat4x4.hpp>

#include <sovren/rhi/Context3D.hpp>

#include <sovren/sg/CameraNode3D.hpp>
#include <sovren/sg/RenderNode.hpp>
#include <sovren/sg/SceneNode.hpp>
#include <sovren/sg/SceneNode3D.hpp>

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

        void renderScene(rhi::Context3D& ctx, sg::RenderNode& scene) {
            if(!objectBuffer_) {
                objectBuffer_ = scene.device()->createBuffer({
                    .size = sizeof(Mat4x4),
                    .cpuAccess = rhi::AccessModes::Write,
                    .gpuAccess = rhi::AccessModes::Read,
                    .bind = rhi::BindModes::ConstantBuffer,
                });
            }
            ctx.setConstantBuffer(objectBuffer_.get(), rhi::ShaderStages::Vertex);
            renderNode(ctx, scene);
        }

    private:
        void renderNode(rhi::Context3D& ctx, sg::SceneNode& n) {
            if(auto* const node3D = dynamic_cast<sg::SceneNode3D*>(&n)) {
                renderNode3D(ctx, *node3D);
            } else {
                renderChildren(ctx, n);
            }
        }

        void renderNode3D(rhi::Context3D& ctx, sg::SceneNode3D& n) {
            transforms_.push(transforms_.top() * n.transform().matrix());

            writeTransform(ctx, transforms_.top() * viewProj_);
            n.render();

            if(auto* const cameraNode = dynamic_cast<sg::CameraNode3D*>(&n)) {
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

        void renderChildren(rhi::Context3D& ctx, sg::SceneNode& n) {
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
