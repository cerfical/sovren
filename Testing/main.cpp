#include <reni/RenderWindow.hpp>

int main() {
	auto window = reni::RenderWindow();
	window.setFillColor({ 255, 255, 0 });

	window.show();
}