#include <gtest/gtest.h>
#include <reni/reni.hpp>

using namespace reni;

class SimpleWindow : public RenderWindow {
public:

	SimpleWindow() {
		m_rect->addChild(rg::makeNode<rg::Rect2D>(
			Vec2(0.0, 0.0),
			Vec2(50.0, 50.0)
		));
		scene().addNode(m_rect);

		m_tri->addChild(rg::makeNode<rg::Triangle3D>(
			Vec3(0.0, 0.0, 50.0),
			Vec3(0.0, 15.0, 50.0),
			Vec3(15.0, 0.0, 50.0)
		));
		scene().addNode(m_tri);
	}

	void onUpdate() override {
		updateRectPos();
		updateTrianglePos();

		RenderWindow::onUpdate();
	}

private:
	void updateRectPos() {
		if(buttonState(MouseButtons::Left)) {
			m_rect->setMatrix(Mat3x3::translation(mousePos()));
		}
	}

	void updateTrianglePos() {
		m_tri->translate(computeTriangleDisplace());
	}

	Vec3 computeTriangleDisplace() const {
		const auto ds = 0.01f;
		Vec3 displace;

		if(keyState(Keys::RightArrow)) {
			displace.x += ds;
		}

		if(keyState(Keys::LeftArrow)) {
			displace.x -= ds;
		}

		if(keyState(Keys::UpArrow)) {
			displace.z += ds;
		}

		if(keyState(Keys::DownArrow)) {
			displace.z -= ds;
		}

		return displace;
	}

	rg::NodePtr<rg::Transform2D> m_rect = rg::makeNode<rg::Transform2D>();
	rg::NodePtr<rg::Transform3D> m_tri = rg::makeNode<rg::Transform3D>();
};

TEST(Render, BasicRendering) {
	SimpleWindow().show();
}
