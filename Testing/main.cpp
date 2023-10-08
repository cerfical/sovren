#include <RENI/RENI.hpp>
#include <iostream>

using namespace RENI;

class SimpleWindow : public RenderWindow {
private:
	void onRender() override {
		renderDevice()->clear({ 255, 0, 0, 255 });
		renderDevice()->drawRect({ Point2D(0, 0), Size2D(100, 100) });
	}
};

int main() {
	auto window = SimpleWindow();
	window.setTitle("Simple Window");
	window.show();

	return Ui::eventLoop();
}
