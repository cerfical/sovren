#include <reni/core/Keys.hpp>
#include <reni/core/MouseButtons.hpp>
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
        static constexpr auto Rotate = 0.005f;

        const auto oldMousePos = std::exchange(mousePos_, mousePos());
        if(buttonState(MouseButtons::Left)) {
            const auto dx = (mousePos_.x - oldMousePos.x) * Rotate;
            const auto dy = (mousePos_.y - oldMousePos.y) * Rotate;

            camera_->rotateY(dx);
            camera_->rotatePitch(dy);
        }

        if(keyState(Keys::RightArrow)) {
            camera_->strafe(Displace);
        }

        if(keyState(Keys::LeftArrow)) {
            camera_->strafe(-Displace);
        }

        if(keyState(Keys::UpArrow)) {
            camera_->walk(Displace);
        }

        if(keyState(Keys::DownArrow)) {
            camera_->walk(-Displace);
        }

        RenderWindow::onUpdate();
    }

    rg::NodePtr<rg::Transform3D> camera_ = rg::makeNode<rg::Transform3D>();
    Vec2 mousePos_;
};


SCENARIO("basic rendering", "[render]") {
    SimpleWindow().show();
}
