#include <reni/core/Keys.hpp>
#include <reni/core/RenderWindow.hpp>

#include <reni/math/Vec3.hpp>

#include <reni/rg/PerspCamera3D.hpp>
#include <reni/rg/RenderNode.hpp>
#include <reni/rg/Transform3D.hpp>
#include <reni/rg/Triangle3D.hpp>

#include <catch2/catch_test_macros.hpp>


using namespace reni;


class SimpleWindow : public RenderWindow {
public:
    SimpleWindow() {
        auto camera = rg::makeNode<rg::PerspCamera3D>();
        camera->setLensSize(size());

        tri_->addChild(rg::makeNode<rg::Triangle3D>(Vec3(0.0, 0.0, 50.0), Vec3(0.0, 15.0, 50.0), Vec3(15.0, 0.0, 50.0)));
        camera->addChild(tri_);
        scene().addNode(camera);
    }

    void onUpdate() override {
        static constexpr auto Displace = 0.01f;

        if(keyState(Keys::RightArrow)) {
            tri_->translateX(Displace);
        }

        if(keyState(Keys::LeftArrow)) {
            tri_->translateX(-Displace);
        }

        if(keyState(Keys::UpArrow)) {
            tri_->translateZ(Displace);
        }

        if(keyState(Keys::DownArrow)) {
            tri_->translateZ(-Displace);
        }

        RenderWindow::onUpdate();
    }

private:
    rg::NodePtr<rg::Transform3D> tri_ = rg::makeNode<rg::Transform3D>();
};


SCENARIO("basic rendering", "[render]") {
    SimpleWindow().show();
}
