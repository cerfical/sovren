#include "sovren/util/Point2.hpp"
#include "sovren/util/Point3.hpp"

#include <sovren/core/Key.hpp>
#include <sovren/core/MouseButton.hpp>
#include <sovren/core/RenderWindow.hpp>

#include <sovren/math/Vec2.hpp>
#include <sovren/math/Vec3.hpp>

#include <sovren/sg/PerspCamera3D.hpp>
#include <sovren/sg/SceneNode.hpp>
#include <sovren/sg/Transform3D.hpp>
#include <sovren/sg/Triangle3D.hpp>

#include <catch2/catch_test_macros.hpp>

#include <utility>


using namespace sovren;


class SimpleWindow : public RenderWindow {
public:

    SimpleWindow() {
        auto camera = makeSceneNode<PerspCamera3D>();
        camera->setViewSize(size());
        camera_->addChild(camera);

        camera->addChild(
            makeSceneNode<Triangle3D>(Point3{ 0.0, 0.0, 50.0 }, Point3{ 0.0, 15.0, 50.0 }, Point3{ 15.0, 0.0, 50.0 })
        );
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

    SceneNodePtr<Transform3D> camera_ = makeSceneNode<Transform3D>();
    Point2 mousePos_;
};


SCENARIO("basic rendering", "[render]") {
    SimpleWindow().show();
}
