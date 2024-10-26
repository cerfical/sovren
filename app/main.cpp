#include <sovren/sovren.hpp>

#include <sovren/sg/PerspCamera3D.hpp>
#include <sovren/sg/Transform3D.hpp>
#include <sovren/sg/Triangle3D.hpp>

using namespace sovren;


class MainWindow : public RenderView {
public:

    MainWindow()
        : camera_(makeSceneNode<Transform3D>()) {

        auto camera = makeSceneNode<PerspCamera3D>();
        camera->setViewSize(size());
        camera_->addChild(camera);

        camera->addChild(
            makeSceneNode<Triangle3D>(Vec3(0.0, 0.0, 50.0), Vec3(0.0, 15.0, 50.0), Vec3(15.0, 0.0, 50.0))
        );
        scene()->addNode(camera_);
    }

private:
    void onUpdate() override {
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

        RenderView::onUpdate();
    }

    void onMouseMove(int dx, int dy) override {
        if(buttonState(MouseButtons::Left)) {
            camera_->rotateY(static_cast<float>(dx) * Rotate);
            camera_->rotatePitch(static_cast<float>(dy) * Rotate);
        }
    }

    static constexpr auto Rotate = 0.003f;
    static constexpr auto Displace = 0.05f;
    SceneNodePtr<Transform3D> camera_;
};

auto main() noexcept -> int {
    MainWindow()
        .show();
}
