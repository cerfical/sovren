#include <RENI/RENI.hpp>
#include <iostream>

using namespace RENI;

int main() {
	auto window = Window();

	window.setTitle("Simple Window");
	window.show();

	return Ui::eventLoop();
}
