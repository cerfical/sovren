#include <RENI/RENI.hpp>
#include <iostream>

using namespace RENI;

class MainWindow : public RenderWindow {
public:

	MainWindow() {
		auto rect = rg::makeNode<rg::Rect2D>();
		rect->topLeft = { 50, 50 };
		rect->bottomRight = { 200, 300 };

		setBackgroundColor({ 255, 255, 255 });
		
		auto scene = lockGraphicsScene();
		scene->addNode(rect);
	}

};

int main() {
	auto window = MainWindow();
	window.setTitle("Simple Window");
	window.show();
	
	return UiEventLoop().run();
}