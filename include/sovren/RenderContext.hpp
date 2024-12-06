#pragma once

#include "math/Mat3x3.hpp"
#include "math/Mat4x4.hpp"
#include "math/Vec2.hpp"

#include "pal/WindowHandle.hpp"
#include "pal/rhi/RenderContext2D.hpp"
#include "pal/rhi/RenderContext3D.hpp"
#include "pal/rhi/RenderDevice.hpp"
#include "pal/rhi/SwapChain.hpp"

#include <memory>
#include <stack>

namespace sovren {

    class RenderContext {
    public:

        class Painter {
        public:

            Painter(const Painter&) = delete;
            auto operator=(const Painter&) -> Painter& = delete;


            void drawLine(Vec2 start, Vec2 end) {
                context2d_->drawLine(start, end);
            }


            void drawRect(Vec2 topLeft, Vec2 bottomRight) {
                context2d_->drawRect(topLeft, bottomRight);
            }


            [[nodiscard]]
            auto transform() const noexcept -> const Mat3x3& {
                return transform2dStack_.top();
            }


            void pushTransform(const Mat3x3& mat) {
                transform2dStack_.push(transform() * mat);
            }


            void popTransform() {
                transform2dStack_.pop();
            }


        private:
            friend RenderContext;

            explicit Painter(std::unique_ptr<rhi::RenderContext2D> ctx2d)
                : context2d_(std::move(ctx2d)) {
                transform2dStack_.push(Mat3x3::identity());
            }


            Painter(Painter&&) = default;
            auto operator=(Painter&&) -> Painter& = default;

            ~Painter() = default;


            std::shared_ptr<rhi::RenderContext2D> context2d_;
            std::stack<Mat3x3> transform2dStack_;
        };


        RenderContext(const RenderContext&) = delete;
        auto operator=(const RenderContext&) -> RenderContext& = delete;

        RenderContext(RenderContext&&) = default;
        auto operator=(RenderContext&&) -> RenderContext& = default;

        ~RenderContext() = default;


        explicit RenderContext(WindowHandle wnd)
            : device_(rhi::RenderDevice::create())
            , swapChain_(device_->createSwapChain(wnd))
            , context3d_(device_->createRenderContext3D())
            , painter_(device_->createRenderContext2D()) {

            transform3dStack_.push(Mat4x4::identity());
        }


        void startFrame() {
            context3d_->startDraw();
            painter_.context2d_->startDraw();

            context3d_->setTarget(swapChain_->frontBuffer());
            painter_.context2d_->setTarget(swapChain_->frontBuffer());
        }


        void endFrame() {
            painter_.context2d_->endDraw();
            context3d_->endDraw();

            swapChain_->swapBuffers();
        }


        [[nodiscard]]
        auto transform() const noexcept -> const Mat4x4& {
            return transform3dStack_.top();
        }


        void pushTransform(const Mat4x4& mat) {
            transform3dStack_.push(transform() * mat);
        }


        void popTransform() {
            transform3dStack_.pop();
        }


        [[nodiscard]]
        auto painter() -> Painter& {
            return painter_;
        }


    private:
        std::unique_ptr<rhi::RenderDevice> device_;
        std::unique_ptr<rhi::SwapChain> swapChain_;

        std::unique_ptr<rhi::RenderContext3D> context3d_;
        std::stack<Mat4x4> transform3dStack_;

        Painter painter_;
    };

}
