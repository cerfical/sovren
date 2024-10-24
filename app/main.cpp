#include <sovren/core/RenderWindow.hpp>

#include <sovren/sg/PerspCamera3D.hpp>
#include <sovren/sg/Transform3D.hpp>
#include <sovren/sg/Triangle3D.hpp>

using namespace sovren;


class SimpleWindow : public RenderWindow {
public:

    SimpleWindow() {
        auto camera = makeSceneNode<PerspCamera3D>();
        camera->setViewSize(size());
        camera_->addChild(camera);

        camera->addChild(
            makeSceneNode<Triangle3D>(Vec3(0.0, 0.0, 50.0), Vec3(0.0, 15.0, 50.0), Vec3(15.0, 0.0, 50.0))
        );
        scene().addNode(camera_);

        mousePos_ = mousePos();
    }

private:
    void onUpdate() override {
        static constexpr auto Displace = 0.05f;

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

    void onMouseMove() override {
        static constexpr auto Rotate = 0.003f;
        const auto oldMousePos = std::exchange(mousePos_, mousePos());

        if(buttonState(MouseButtons::Left)) {
            const auto dx = static_cast<float>(mousePos_.x - oldMousePos.x) * Rotate;
            const auto dy = static_cast<float>(mousePos_.y - oldMousePos.y) * Rotate;

            camera_->rotateY(dx);
            camera_->rotatePitch(dy);
        }
    }

    SceneNodePtr<Transform3D> camera_ = makeSceneNode<Transform3D>();
    Point2 mousePos_;
};

auto main() noexcept -> int {
    SimpleWindow()
        .show();
}
