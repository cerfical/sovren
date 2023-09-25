#include <RENI/RENI.hpp>
#include <iostream>

using namespace RENI;

int main() {
	auto window = Window();

	window.setTitle("Simple Window");
	window.show();

	auto rd = window.renderDevice();
	rd->startDraw();
	rd->clear({ 255, 0, 0, 255 });
	rd->drawLine({ {0, 0}, {100, 100} });
	rd->endDraw();

	return Ui::eventLoop();
}
