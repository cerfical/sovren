#include <RENI/RENI.hpp>
#include <iostream>

using namespace RENI;

class SimpleWindow : public RenderWindow {
private:
	void onRender() override {
		renderDevice()->setDrawColor({128, 255, 128, 255});
		renderDevice()->fillRect({ Point2D(50, 50), Size2D(200, 300) });
	}
};

int main() {
	auto window = SimpleWindow();
	window.setTitle("Simple Window");
	window.show();

	return UiMainLoop::get()->exec();
}
