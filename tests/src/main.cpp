#include <reni/reni.hpp>

#include <iostream>

using namespace reni;

class SimpleWindow : public RenderWindow {
public:

	SimpleWindow() {
		m_rect = rg::makeNode<rg::Rect2D>();
		m_rect->size = { 20, 20 };
		m_rect->topLeft = mousePos();
		
		scene().addNode(m_rect);
	}

private:
	void onMouseMove(Point2 newPos, Point2) override {
		m_rect->topLeft = newPos;
	}

	rg::NodePtr<rg::Rect2D> m_rect;
};

int main() {
	SimpleWindow window;
	window.setFillColor({ 1.0f, 0.0f, 1.0f });
	window.show();
}