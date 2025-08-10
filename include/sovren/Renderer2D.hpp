#pragma once

#include "math/Mat3x3.hpp"
#include "rhi/Context2D.hpp"
#include "util/Color.hpp"

#include "RenderNode.hpp"
#include "SceneNode.hpp"
#include "SceneNode2D.hpp"

#include <memory>
#include <stack>

namespace sovren {

    class Renderer2D {
    public:
        Renderer2D() {
            // the root transform is always an identity matrix
            transforms_.push(Mat3x3::identity());
        }

        Renderer2D(const Renderer2D&) = delete;
        auto operator=(const Renderer2D&) -> Renderer2D& = delete;

        Renderer2D(Renderer2D&&) = default;
        auto operator=(Renderer2D&&) -> Renderer2D& = default;

        ~Renderer2D() = default;

        void renderScene(RenderNode& scene) {
            auto& ctx = *scene.context2D();
            ctx.setRenderTarget(scene.target());
            ctx.beginDraw();

            ctx.clear(Color::white());
            renderNode(ctx, scene);

            ctx.endDraw();
            ctx.setRenderTarget(nullptr);
        }

    private:
        void renderNode(rhi::Context2D& ctx, SceneNode& n) {
            if(auto* const node2D = dynamic_cast<SceneNode2D*>(&n)) {
                renderNode2D(ctx, *node2D);
            } else {
                renderChildren(ctx, n);
            }
        }

        void renderNode2D(rhi::Context2D& ctx, SceneNode2D& n) {
            transforms_.push(transforms_.top() * n.transform().matrix());

            const auto& m = transforms_.top();
            ctx.setTransform({
                { m[0][0], m[0][1], m[1][0], m[1][1], m[2][0], m[2][1] },
            });

            n.render();
            renderChildren(ctx, n);

            transforms_.pop();
        }

        void renderChildren(rhi::Context2D& ctx, SceneNode& n) {
            for(const auto& child : n.children()) {
                renderNode(ctx, *child);
            }
        }

        std::stack<Mat3x3> transforms_;
    };

}
