#include "core/Renderer.hpp"
#include "core/SceneGraph.hpp"

#include "pal/Window.hpp"

#include "math/Mat3x3.hpp"
#include "math/Mat4x4.hpp"

#include "rhi/RenderBackend.hpp"
#include "rhi/RenderContext.hpp"
#include "rhi/SwapChain.hpp"

#include "sg/Camera3D.hpp"
#include "sg/Line2D.hpp"
#include "sg/NodeVisitor.hpp"
#include "sg/Rect2D.hpp"
#include "sg/SceneNode.hpp"
#include "sg/Transform2D.hpp"
#include "sg/Transform3D.hpp"
#include "sg/Triangle3D.hpp"

#include <stack>
#include <unordered_map>

namespace sovren {
    struct Renderer::Impl : private NodeVisitor {

        void renderScene(const SceneGraph& scene) {
            // start with no-op transformations
            transformStack2d_.push(Mat3x3::identity());
            transformStack3d_.push(Mat4x4::identity());
            viewProjStack_.push(Mat4x4::identity());

            visitNodes(scene.nodes());

            viewProjStack_.pop();
            transformStack3d_.pop();
            transformStack2d_.pop();
        }


        std::unique_ptr<RenderBackend> renderApi;
        std::unique_ptr<RenderContext> renderContext;
        std::unique_ptr<SwapChain> swapChain;

        Color clearColor = Color::fromRgb(1.0f, 1.0f, 1.0f); // clear the window with white color by default


    private:
        void visit(const Line2D& l) override {
            setupRender2d();
            renderContext->drawLine(l.startPoint(), l.endPoint());

            visitChildren(l);
        }

        void visit(const Rect2D& r) override {
            setupRender2d();
            renderContext->drawRect(r.topLeftPoint(), r.botRightPoint());

            visitChildren(r);
        }

        void setupRender2d() {
            renderContext->setTransformMatrix(transformStack2d_.top());
        }

        void visit(const Transform2D& t) override {
            // make 2D geometry child nodes to render relative to their parent
            transformStack2d_.push(transformStack2d_.top() * t.toMatrix());
            visitChildren(t);
            transformStack2d_.pop();
        }


        void visit(const Triangle3D& t) override {
            const auto [it, ok] = meshes_.try_emplace(&t);
            auto& mesh = it->second;
            if(ok) {
                mesh = renderApi->createVertexBuffer(std::as_bytes(t.points()));
            }

            setupRender3d();
            renderContext->drawMesh(*mesh);
            visitChildren(t);
        }

        void setupRender3d() {
            renderContext->setTransformMatrix(transformStack3d_.top() * viewProjStack_.top());
        }

        void visit(const Transform3D& t) override {
            // make 3D geometry child nodes to render relative to their parent
            transformStack3d_.push(transformStack3d_.top() * t.toMatrix());
            visitChildren(t);
            transformStack3d_.pop();
        }

        void visit(const Camera3D& c) override {
            viewProjStack_.push(
                invert(transformStack3d_.top()) * // calculate the camera view matrix based on position in the scene
                c.toProjMatrix()                  // then apply the projection matrix
            );

            // make all child nodes to be positioned independently of the camera
            transformStack3d_.push(Mat4x4::identity());

            visitChildren(c);

            transformStack3d_.pop();
            viewProjStack_.pop();
        }


        void visitChildren(const SceneNode& n) {
            visitNodes(n.children());
        }

        void visitNodes(const SceneNodeList& n) {
            for(const auto& c : n) {
                c->acceptVisitor(*this);
            }
        }


        std::unordered_map<const SceneNode*, std::unique_ptr<VertexBuffer>> meshes_;

        std::stack<Mat3x3> transformStack2d_;
        std::stack<Mat4x4> transformStack3d_;
        std::stack<Mat4x4> viewProjStack_;
    };


    Renderer::Renderer(Window& window)
        : impl_(std::make_unique<Impl>()) {

        impl_->renderApi = RenderBackend::create();
        impl_->swapChain = impl_->renderApi->createSwapChain(window.nativeHandle());
        impl_->renderContext = impl_->renderApi->createRenderContext();
    }


    void Renderer::renderScene(const SceneGraph& scene) {
        impl_->renderContext->startRender(impl_->swapChain->frontBuffer());
        impl_->renderContext->clear(impl_->clearColor);

        impl_->renderScene(scene);

        impl_->renderContext->endRender();
        impl_->swapChain->swapBuffers();
    }


    void Renderer::setRenderSize(Size2 s) {
        impl_->swapChain->setBufferSize(s);
    }


    void Renderer::setClearColor(Color clearCol) {
        impl_->clearColor = clearCol;
    }


    Color Renderer::clearColor() const {
        return impl_->clearColor;
    }


    Renderer::Renderer(Renderer&&) noexcept = default;
    Renderer& Renderer::operator=(Renderer&&) noexcept = default;

    Renderer::~Renderer() = default;
}
