#pragma once

#include <sovren/log/ConsoleLogger.hpp>
#include <sovren/log/Log.hpp>

#include <sovren/pal/EventPump.hpp>
#include <sovren/pal/Key.hpp>
#include <sovren/pal/MouseButton.hpp>
#include <sovren/pal/Window.hpp>
#include <sovren/pal/WindowHandler.hpp>

#include <sovren/rhi/Context2D.hpp>
#include <sovren/rhi/Context3D.hpp>
#include <sovren/rhi/Device.hpp>
#include <sovren/rhi/SwapChain.hpp>

#include <sovren/sg/SceneNode.hpp>
#include <sovren/sg/SceneNode2D.hpp>
#include <sovren/sg/SceneNode3D.hpp>
#include <sovren/sg/WindowNode.hpp>

#include <sovren/util/Point2.hpp>
#include <sovren/util/Size2.hpp>

#include <sovren/RenderQueue2D.hpp>
#include <sovren/RenderQueue3D.hpp>

#include <algorithm>
#include <memory>
#include <optional>
#include <string>
#include <utility>

namespace sovren::sg {

    class RootNode : public SceneNode {
    public:
        void exec() {
            init();

            while(true) {
                const auto windows = collectVisibleWindows();
                if(windows.empty()) {
                    break;
                }

                events_->pumpEvents();
                update();
                cleanupWindows();

                for(const auto& wnd : windows_) {
                    wnd->syncWithNode();
                    renderWindow(*wnd);
                }
            }

            free();
        }

        void init() override {
            log_ = log::Log(std::make_shared<log::ConsoleLogger>());
            palLog_ = log_.withTags("PAL");
            log_.info("Log initialized");

            events_ = pal::EventPump::create(palLog_);
            device_ = rhi::Device::create();

            context2D_ = device_->createContext2D();
            context3D_ = device_->createContext3D();

            queue3D_.emplace(device_);
            queue2D_.emplace();

            SceneNode::init();
        }

        void free() override {
            SceneNode::free();

            queue2D_.reset();
            queue3D_.reset();

            context3D_.reset();
            context2D_.reset();

            windows_.clear();
            device_.reset();
            events_.reset();

            log_.info("Log closed");
            log_ = palLog_ = log::Log();
        }

        [[nodiscard]]
        auto log() const noexcept -> log::Log* override {
            return &log_;
        }

    private:
        class Window final : public pal::WindowHandler {
        public:
            explicit Window(std::weak_ptr<WindowNode> node, rhi::Device& device, log::Log log)
                : node_(std::move(node)) {
                window_ = pal::Window::create(this, std::move(log));

                swapChain_ = device.createSwapChain(window_->handle());
                size_ = window_->size();

                syncWithNode();
            }

            void syncWithNode() {
                if(const auto n = node_.lock()) {
                    setVisible(n->isVisible());
                    setSize(n->size());
                    setTitle(n->title());
                }
            }

            [[nodiscard]]
            auto swapChain() const noexcept -> rhi::SwapChain* {
                return swapChain_.get();
            }

            [[nodiscard]]
            auto node() const noexcept -> WindowNode* {
                if(const auto n = node_.lock()) {
                    return n.get();
                }
                return nullptr;
            }

            [[nodiscard]]
            auto isVisible() const noexcept -> bool {
                return visible_;
            }

            [[nodiscard]]
            auto size() const noexcept -> Size2i {
                return size_;
            }

        private:
            void windowClose() override {
                event(&RootNode::onWindowClose);
            }

            void windowResize() override {
                const auto newSize = window_->size();
                size_ = newSize;

                swapChain_->resizeBuffers(newSize);
                event(&RootNode::onWindowResize, newSize);
            }

            void keyDown(pal::Key k) override {
                event(&RootNode::onKeyDown, k);
            }

            void keyUp(pal::Key k) override {
                event(&RootNode::onKeyUp, k);
            }

            void mouseButtonDown(pal::MouseButton b) override {
                event(&RootNode::onMouseButtonDown, b);
            }

            void mouseButtonUp(pal::MouseButton b) override {
                event(&RootNode::onMouseButtonUp, b);
            }

            void mouseMove() override {
                event(&RootNode::onMouseMove, window_->mousePos());
            }

            template <typename... Params, typename... Args>
            void event(void (SceneNode::*event)(Params...), Args&&... args) {
                if(const auto n = node_.lock()) {
                    (n.get()->*event)(std::forward<Args>(args)...);
                }
            }

            void setTitle(std::string title) {
                if(title != title_) {
                    window_->setTitle(title);
                    title_ = std::move(title);
                }
            }

            void setSize(Size2i size) {
                if(size != size_) {
                    window_->setSize(size);
                    size_ = size;
                }
            }

            void setVisible(bool visible) {
                if(visible_ != visible) {
                    window_->setVisible(visible);
                    visible_ = visible;
                }
            }

            std::unique_ptr<pal::Window> window_;
            std::weak_ptr<WindowNode> node_;

            std::unique_ptr<rhi::SwapChain> swapChain_;

            std::string title_;
            Size2i size_;
            bool visible_ = {};
        };

        using WindowPtr = std::shared_ptr<Window>;
        using WindowList = std::vector<WindowPtr>;

        [[nodiscard]]
        auto collectVisibleWindows() -> WindowList {
            WindowList windows;

            const auto collect = [this, &windows](this const auto& self, const auto& n) -> void {
                for(const auto& child : n.children()) {
                    const auto wndNode = std::dynamic_pointer_cast<WindowNode>(child);
                    if(!wndNode) {
                        // if the node is not a window node, recurse to children
                        self(*child);
                        continue;
                    }

                    const auto it = std::ranges::find_if(windows_, [&wndNode](const auto& wnd) {
                        return wnd->node() == wndNode.get();
                    });

                    // create a new window for the node if it doesn't already exist
                    WindowPtr wnd;
                    if(it != windows_.end()) {
                        wnd = *it;
                    } else {
                        wnd = std::make_shared<Window>(wndNode, *device_, palLog_);
                        windows_.push_back(wnd);
                    }

                    if(wnd->isVisible()) {
                        windows.push_back(wnd);
                    }
                }
            };
            collect(*this);

            return windows;
        }

        void cleanupWindows() noexcept {
            // remove any windows that are no longer in the scene
            std::erase_if(windows_, [](const auto& wnd) {
                return wnd->node() == nullptr;
            });
        }

        void renderWindow(const Window& wnd) {
            renderNode(*wnd.node());

            // render 3D scene
            context3D_->setRenderTarget(wnd.swapChain()->frontBuffer());
            context3D_->setViewport({ { 0, 0 }, static_cast<Size2f>(wnd.size()) });

            if(const auto bgColor = wnd.node()->backgroundColor(); bgColor) {
                context3D_->clear(bgColor);
            }
            queue3D_->flush(*context3D_);

            context3D_->setRenderTarget(nullptr);

            // render 2D scene
            context2D_->setRenderTarget(wnd.swapChain()->frontBuffer());
            context2D_->beginDraw();

            queue2D_->flush(*context2D_);

            context2D_->endDraw();
            context2D_->setRenderTarget(nullptr);

            wnd.swapChain()->swapBuffers();
        }

        void renderNode(const sg::SceneNode& n) {
            if(const auto* const node2D = dynamic_cast<const sg::SceneNode2D*>(&n)) {
                node2D->render(*queue2D_);
            }
            if(const auto* const node3D = dynamic_cast<const sg::SceneNode3D*>(&n)) {
                node3D->render(*queue3D_);
            }
            renderChildren(n);
        }

        void renderChildren(const sg::SceneNode& n) {
            for(const auto& child : n.children()) {
                renderNode(*child);
            }
        }

        WindowList windows_;

        std::unique_ptr<pal::EventPump> events_;
        std::shared_ptr<rhi::Device> device_;

        std::unique_ptr<rhi::Context2D> context2D_;
        std::unique_ptr<rhi::Context3D> context3D_;

        std::optional<RenderQueue2D> queue2D_;
        std::optional<RenderQueue3D> queue3D_;

        mutable log::Log log_;
        log::Log palLog_;
    };

}
