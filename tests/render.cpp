#include "reni/util/Point2.hpp"

#include <reni/core/Key.hpp>
#include <reni/core/MouseButton.hpp>
#include <reni/core/RenderWindow.hpp>

#include <reni/math/Vec2.hpp>
#include <reni/math/Vec3.hpp>

#include <reni/rg/PerspCamera3D.hpp>
#include <reni/rg/RenderNode.hpp>
#include <reni/rg/Transform3D.hpp>
#include <reni/rg/Triangle3D.hpp>

#include <catch2/catch_test_macros.hpp>

#include <utility>


using namespace reni;


class SimpleWindow : public RenderWindow {
public:

    SimpleWindow() {
        auto camera = rg::makeNode<rg::PerspCamera3D>();
        camera->setLensSize(size());
        camera_->addChild(camera);

        camera->addChild(rg::makeNode<rg::Triangle3D>(Vec3(0.0, 0.0, 50.0), Vec3(0.0, 15.0, 50.0), Vec3(15.0, 0.0, 50.0)));
        scene().addNode(camera_);

        mousePos_ = mousePos();
    }

private:
    void onUpdate() override {
        static constexpr auto Displace = 0.05f;

        if(keyState(Key::RightArrow)) {
            camera_->strafe(Displace);
        }

        if(keyState(Key::LeftArrow)) {
            camera_->strafe(-Displace);
        }

        if(keyState(Key::UpArrow)) {
            camera_->walk(Displace);
        }

        if(keyState(Key::DownArrow)) {
            camera_->walk(-Displace);
        }

        RenderWindow::onUpdate();
    }

    void onMouseMove() override {
        static constexpr auto Rotate = 0.003f;
        const auto oldMousePos = std::exchange(mousePos_, mousePos());

        if(buttonState(MouseButton::Left)) {
            const auto dx = (mousePos_.x - oldMousePos.x) * Rotate;
            const auto dy = (mousePos_.y - oldMousePos.y) * Rotate;

            camera_->rotateY(dx);
            camera_->rotatePitch(dy);
        }
    }

    rg::NodePtr<rg::Transform3D> camera_ = rg::makeNode<rg::Transform3D>();
    Point2 mousePos_;
};


SCENARIO("basic rendering", "[render]") {
    SimpleWindow().show();
}
