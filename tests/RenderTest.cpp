#include <gtest/gtest.h>
#include <reni/reni.hpp>

using namespace reni;

class SimpleWindow : public RenderWindow {
public:

	SimpleWindow() {
		auto camera = rg::makeNode<rg::PerspCamera3D>();
		camera->setLensSize(size());

		m_tri->addChild(rg::makeNode<rg::Triangle3D>(
			Vec3(0.0, 0.0, 50.0),
			Vec3(0.0, 15.0, 50.0),
			Vec3(15.0, 0.0, 50.0)
		));
		camera->addChild(m_tri);
		scene().addNode(camera);
	}

	void onUpdate() override {
		static constexpr auto ds = 0.01f;

		if(keyState(Keys::RightArrow)) {
			m_tri->translateX(ds);
		}
		
		if(keyState(Keys::LeftArrow)) {
			m_tri->translateX(-ds);
		}

		if(keyState(Keys::UpArrow)) {
			m_tri->translateZ(ds);
		}
		
		if(keyState(Keys::DownArrow)) {
			m_tri->translateZ(-ds);
		}

		RenderWindow::onUpdate();
	}

private:
	rg::NodePtr<rg::Transform3D> m_tri = rg::makeNode<rg::Transform3D>();
};

TEST(Render, BasicRendering) {
	SimpleWindow().show();
}
