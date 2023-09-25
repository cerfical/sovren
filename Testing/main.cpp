#include <RENI/RENI.hpp>
#include <iostream>

using namespace RENI;

class SimpleWindow : public Window {
private:
	void OnResize(const Size2D& newSize, const Size2D& oldSize) override {
		std::cout << "resized from " << oldSize << " to " << newSize << std::endl;
	}

	void OnKeyPress(Keys pressedKey) override {
		std::cout << pressedKey << " was pressed" << std::endl;
	}
};

int main() {
	SimpleWindow window;
	window.SetTitle("Simple Window");
	window.SetVisible(true);
	
	return EventLoop().Run();
}