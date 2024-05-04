#include <reni/reni.hpp>

#include <iostream>

using namespace reni;

class SimpleWindow : public RenderWindow {
public:

	SimpleWindow() {
		moveRect(mousePos());
		scene().addNode(m_rect);
	}

private:
	void onMouseMove(Point2D newPos, Point2D oldPos) override {
		moveRect(newPos);
	}

	void moveRect(Point2D pos) {
		m_rect->topLeft = m_rect->bottomRight = pos;
		m_rect->bottomRight.x += 20;
		m_rect->bottomRight.y += 20;
	}

	rg::NodePtr<rg::Rect2D> m_rect = rg::makeNode<rg::Rect2D>();
};

int main() {
	SimpleWindow window;
	window.setFillColor({ 255, 0, 255 });
	window.show();
}