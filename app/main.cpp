#include <sovren/sovren.hpp>

using namespace sovren;


class MainWindow : public RenderView {
public:

    MainWindow() {
        camera_ = makeSceneNode<PerspCamera3D>();
        camera_->setViewSize(size());

        camera_->addChild(makeSceneNode<MeshNode3D>(std::vector{ Vec3(0, 0, 50), Vec3(0, 15, 50), Vec3(15, 0, 50) }));
        scene()->addNode(camera_);
    }

private:
    void onUpdate() override {
        if(keyState(Keys::RightArrow)) {
            camera_->transform().strafe(Displace);
        }

        if(keyState(Keys::LeftArrow)) {
            camera_->transform().strafe(-Displace);
        }

        if(keyState(Keys::UpArrow)) {
            camera_->transform().walk(Displace);
        }

        if(keyState(Keys::DownArrow)) {
            camera_->transform().walk(-Displace);
        }

        RenderView::onUpdate();
    }

    void onMouseMove(int dx, int dy) override {
        if(buttonState(MouseButtons::Left)) {
            camera_->transform().rotateY(static_cast<float>(dx) * Rotate);
            camera_->transform().rotatePitch(static_cast<float>(dy) * Rotate);
        }
    }


    static constexpr auto Rotate = 0.003f;
    static constexpr auto Displace = 0.05f;

    SceneNodePtr<Camera3D> camera_;
};

auto main() noexcept -> int {
    MainWindow()
        .show();
}
