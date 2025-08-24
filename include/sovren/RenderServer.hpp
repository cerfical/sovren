#pragma once

#include <sovren/log/Log.hpp>

#include <sovren/pal/EventPump.hpp>
#include <sovren/pal/Key.hpp>
#include <sovren/pal/MouseButton.hpp>
#include <sovren/pal/Window.hpp>
#include <sovren/pal/WindowHandler.hpp>

#include <sovren/rhi/AccessModes.hpp>
#include <sovren/rhi/BindModes.hpp>
#include <sovren/rhi/Buffer.hpp>
#include <sovren/rhi/Context2D.hpp>
#include <sovren/rhi/Context3D.hpp>
#include <sovren/rhi/Device.hpp>
#include <sovren/rhi/Pipeline.hpp>
#include <sovren/rhi/ShaderStages.hpp>
#include <sovren/rhi/SwapChain.hpp>

#include <sovren/Event.hpp>
#include <sovren/KeyEventType.hpp>
#include <sovren/MouseEventType.hpp>
#include <sovren/RenderList.hpp>
#include <sovren/Window.hpp>

#include <algorithm>
#include <memory>
#include <span>
#include <unordered_map>
#include <vector>

namespace sovren {

    class RenderServer {
    public:
        explicit RenderServer(log::Log log = {});

        RenderServer(const RenderServer&) = delete;
        auto operator=(const RenderServer&) -> RenderServer& = delete;

        RenderServer(RenderServer&&) = default;
        auto operator=(RenderServer&&) -> RenderServer& = default;

        ~RenderServer() {
            log_.info("Render server closed");
        }

        void enqueue(RenderList list) {
            lists_.emplace_back(std::move(list));
        }

        [[nodiscard]]
        auto isActive() const -> bool {
            return std::ranges::any_of(windows_, [](const auto& w) {
                return w->window()->isVisible();
            });
        }

        void renderFrame() {
            for(const auto& list : lists_) {
                const auto* const wndData = getWindowData(list.targetWindow());

                render3D(list, *wndData);
                render2D(list, *wndData);

                wndData->swapChain()->swapBuffers();
            }
            lists_.clear();

            eventPump_->pumpEvents();
            for(auto& wnd : windows_) {
                wnd->sync();
            }
        }

        void registerWindow(Window* wnd) {
            auto* const data = getWindowData(wnd);
            if(data) {
                data->addRef();
                return;
            }
            windows_.emplace_back(std::make_unique<WindowData>(wnd, *device_, palLog_));
        }

        void unregisterWindow(Window* wnd) {
            const auto it = std::ranges::find_if(windows_, [wnd](const auto& w) {
                return w->window() == wnd;
            });
            if(it == windows_.end()) {
                return;
            }

            // destroy the window if was the last reference
            auto* const data = it->get();
            if(data->releaseRef()) {
                windows_.erase(it);
            }
        }

        void registerMesh(const Mesh* mesh) {
            auto* const gpuMesh = getGpuMesh(mesh);
            if(gpuMesh) {
                gpuMesh->addRef();
                return;
            }

            gpuMeshes_.emplace(
                std::piecewise_construct,
                std::forward_as_tuple(mesh),
                std::forward_as_tuple(mesh, *device_)
            );
        }

        void unregisterMesh(const Mesh* mesh) {
            const auto it = gpuMeshes_.find(mesh);
            if(it == gpuMeshes_.end()) {
                return;
            }

            if(it->second.releaseRef()) {
                gpuMeshes_.erase(it);
            }
        }

    private:
        class RefCounted {
        public:
            void addRef() noexcept {
                ++refCount_;
            }

            [[nodiscard]]
            auto releaseRef() noexcept -> bool {
                refCount_--;
                return refCount_ == 0;
            }

        private:
            std::size_t refCount_ = 1;
        };

        class WindowData final : public RefCounted, public pal::WindowHandler {
        public:
            explicit WindowData(Window* wnd, rhi::Device& device, log::Log log)
                : wnd_(wnd) {
                palWindow_ = pal::Window::create(this, std::move(log));

                swapChain_ = device.createSwapChain(palWindow_->handle());
                size_ = palWindow_->size();
            }

            void sync() {
                setVisible(wnd_->isVisible());
                setSize(wnd_->size());
                setTitle(wnd_->title());
            }

            [[nodiscard]]
            auto swapChain() const noexcept -> rhi::SwapChain* {
                return swapChain_.get();
            }

            [[nodiscard]]
            auto isVisible() const noexcept -> bool {
                return visible_;
            }

            [[nodiscard]]
            auto size() const noexcept -> Size2i {
                return size_;
            }

            [[nodiscard]]
            auto window() const noexcept -> Window* {
                return wnd_;
            }

        private:
            void windowClose() override {
                wnd_->events().push(WindowEventType::Close);
            }

            void windowResize() override {
                size_ = palWindow_->size();

                swapChain_->resizeBuffers(size_);
                wnd_->events().push(Event(WindowEventType::Resize, size_));
            }

            void keyDown(pal::Key k) override {
                wnd_->events().push(Event(KeyEventType::KeyDown, k));
            }

            void keyUp(pal::Key k) override {
                wnd_->events().push(Event(KeyEventType::KeyUp, k));
            }

            void mouseButtonDown(pal::MouseButton b) override {
                wnd_->events().push(Event(MouseEventType::ButtonDown, b));
            }

            void mouseButtonUp(pal::MouseButton b) override {
                wnd_->events().push(Event(MouseEventType::ButtonUp, b));
            }

            void mouseMove() override {
                wnd_->events().push(Event(MouseEventType::Move, {}, palWindow_->mousePos()));
            }

            void setTitle(std::string title) {
                if(title != title_) {
                    palWindow_->setTitle(title);
                    title_ = std::move(title);
                }
            }

            void setSize(Size2i size) {
                if(size != size_) {
                    palWindow_->setSize(size);
                    size_ = size;
                }
            }

            void setVisible(bool visible) {
                if(visible_ != visible) {
                    palWindow_->setVisible(visible);
                    visible_ = visible;
                }
            }

            std::unique_ptr<pal::Window> palWindow_;
            Window* wnd_ = {};

            std::unique_ptr<rhi::SwapChain> swapChain_;

            std::string title_;
            Size2i size_;
            bool visible_ = {};
        };

        struct GpuMesh : public RefCounted {
            GpuMesh(const Mesh* m, rhi::Device& device) {
                vertices = device.createBuffer({
                    .data = as_bytes(m->vertices()),
                    .gpuAccess = rhi::AccessModes::Read,
                    .bind = rhi::BindModes::VertexBuffer,
                });
            }

            std::unique_ptr<rhi::Buffer> vertices;
        };

        [[nodiscard]]
        auto getWindowData(Window* wnd) -> WindowData* {
            auto it = std::ranges::find_if(windows_, [wnd](const auto& w) {
                return w->window() == wnd;
            });
            if(it != windows_.end()) {
                return it->get();
            }
            return nullptr;
        }

        [[nodiscard]]
        auto getGpuMesh(const Mesh* mesh) -> GpuMesh* {
            if(const auto it = gpuMeshes_.find(mesh); it != gpuMeshes_.end()) {
                return &it->second;
            }
            return nullptr;
        }

        void render2D(const RenderList& list, const WindowData& wnd) {
            context2D_->setRenderTarget(wnd.swapChain()->frontBuffer());
            context2D_->beginDraw();

            for(const auto& item : list.items2D()) {
                const auto& m = item.transform.matrix();
                context2D_->setTransform({
                    { m[0][0], m[0][1], m[1][0], m[1][1], m[2][0], m[2][1] },
                });
                context2D_->setOutlineColor(item.outlineColor);

                if(const auto* const r = item.shape.as<Rect>()) {
                    context2D_->drawRect(
                        r->topLeft, { r->topLeft.x + r->size.width, r->topLeft.y + r->size.height }
                    );
                } else if(const auto* const l = item.shape.as<Line>()) {
                    context2D_->drawLine(l->start, l->end);
                }
            }

            context2D_->endDraw();
            context2D_->setRenderTarget(nullptr);
        }

        void render3D(const RenderList& list, const WindowData& wnd) {
            context3D_->setRenderTarget(wnd.swapChain()->frontBuffer());
            context3D_->setViewport({ { 0, 0 }, static_cast<Size2f>(wnd.size()) });

            if(const auto bgColor = list.targetWindow()->backgroundColor(); bgColor) {
                context3D_->clear(bgColor);
            }

            for(const auto& item : list.items3D()) {
                const auto* const gpuMesh = getGpuMesh(item.mesh);

                context3D_->setPipeline(pipeline_.get());
                context3D_->setVertexBuffer(gpuMesh->vertices.get(), 12);
                context3D_->setConstantBuffer(objectBuffer_.get(), rhi::ShaderStages::Vertex);

                writeTransform(item.transform.matrix() * list.cameraMatrix());
                context3D_->draw(item.mesh->size());
            }

            context3D_->setRenderTarget(nullptr);
        }

        void writeTransform(const Mat4x4& m) {
            auto buf = context3D_->map(objectBuffer_.get(), rhi::AccessModes::Write);
            *reinterpret_cast<Mat4x4*>(buf.data()) = m;
            context3D_->unmap(objectBuffer_.get());
        }

        log::Log log_;
        log::Log palLog_;

        std::vector<std::unique_ptr<WindowData>> windows_;
        std::unique_ptr<pal::EventPump> eventPump_;

        std::unique_ptr<rhi::Device> device_;
        std::unique_ptr<rhi::Context3D> context3D_;
        std::unique_ptr<rhi::Context2D> context2D_;

        std::unique_ptr<rhi::Pipeline> pipeline_;
        std::unique_ptr<rhi::Buffer> objectBuffer_;

        std::unordered_map<const Mesh*, GpuMesh> gpuMeshes_;

        std::vector<RenderList> lists_;
    };

}
