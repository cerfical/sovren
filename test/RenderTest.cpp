#include <gtest/gtest.h>
#include <reni/reni.hpp>

using namespace reni;

class SimpleWindow : public RenderWindow {
public:

	SimpleWindow() {
		const auto tri = rg::makeNode<rg::Triangle3D>(
			Vec3(0.0f, 0.0f, 500.0f),
			Vec3(0.0f, 50.0f, 500.0f),
			Vec3(50.0f, 50.0f, 500.0f)
		);

		const auto rect = rg::makeNode<rg::Rect2D>(
			Vec2(10.0f, 10.0f),
			Vec2(70.0f, 70.0f)
		);

		scene().addNode(tri);
		scene().addNode(rect);
	}

};

TEST(Render, BasicRendering) {
	SimpleWindow().show();
}
