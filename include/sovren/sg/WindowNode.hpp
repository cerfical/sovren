#pragma once

#include <sovren/pal/Key.hpp>
#include <sovren/pal/MouseButton.hpp>

#include <sovren/sg/SceneNode.hpp>

#include <sovren/util/Color.hpp>
#include <sovren/util/Point2.hpp>
#include <sovren/util/Size2.hpp>

#include <sovren/Input.hpp>

#include <string>

namespace sovren::sg {

    class WindowNode : public SceneNode {
    public:
        void init() override {
            SceneNode::init();
            show();
        }

        void free() override {
            hide();
            SceneNode::free();
        }

        [[nodiscard]]
        auto input() noexcept -> const Input* override {
            return &input_;
        }

        void setTitle(std::string title) noexcept {
            title_ = std::move(title);
        }

        [[nodiscard]]
        auto title() const noexcept -> const std::string& {
            return title_;
        }

        void setSize(Size2i size) noexcept {
            size_ = size;
        }

        [[nodiscard]]
        auto size() const noexcept -> Size2i {
            return size_;
        }

        [[nodiscard]]
        auto width() const noexcept -> int {
            return size().width;
        }

        [[nodiscard]]
        auto height() const noexcept -> int {
            return size().height;
        }

        void setVisible(bool visible) noexcept {
            visible_ = visible;
        }

        void show() noexcept {
            setVisible(true);
        }

        void hide() noexcept {
            setVisible(false);
        }

        [[nodiscard]]
        auto isVisible() const noexcept -> bool {
            return visible_;
        }

        void setBackgroundColor(Color c) noexcept {
            bgColor_ = c;
        }

        [[nodiscard]]
        auto backgroundColor() const noexcept -> Color {
            return bgColor_;
        }

    protected:
        void onWindowResize(Size2i size) override {
            size_ = size;
            SceneNode::onWindowResize(size);
        }

        void onWindowClose() override {
            SceneNode::onWindowClose();
            hide();
        }

        void onKeyDown(pal::Key k) override {
            if(input_.isUp(k)) {
                input_.press(k);
                SceneNode::onKeyDown(k);
            }
        }

        void onKeyUp(pal::Key k) override {
            if(input_.isDown(k)) {
                input_.release(k);
                SceneNode::onKeyUp(k);
            }
        }

        void onMouseButtonDown(pal::MouseButton b) override {
            if(input_.isUp(b)) {
                input_.press(b);
                SceneNode::onMouseButtonDown(b);
            }
        }

        void onMouseButtonUp(pal::MouseButton b) override {
            if(input_.isDown(b)) {
                input_.release(b);
                SceneNode::onMouseButtonUp(b);
            }
        }

        void onMouseMove(Point2i pos) override {
            const auto oldPos = input_.mousePos();
            if(oldPos != pos) {
                input_.setMousePos(pos);
                SceneNode::onMouseMove(pos);
            }
        }

    private:
        static constexpr auto DefaultSize = Size2i(1024, 640);
        static constexpr auto DefaultBgColor = Color::white();

        std::string title_;
        Color bgColor_ = DefaultBgColor;
        Size2i size_ = DefaultSize;
        bool visible_ = {};

        Input input_;
    };

}
