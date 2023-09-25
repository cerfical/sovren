#include <RENI/RENI.hpp>
#include <iostream>

using namespace RENI;

class RenderWindow : public Window {
private:
	void onMousePress(const MousePressEvent& e) override {
		if(e[MouseButtons::Left]) {
			captureMouse();
		}
	}

	void onMouseRelease(const MouseReleaseEvent& e) override {
		if(e[MouseButtons::Left]) {
			releaseMouse();
		}
	}

	void onClose(const CloseEvent&) override {
		setVisible(false);
	}
};

int main() {
	RenderWindow window;
	window.setTitle("Window");
	window.setVisible(true);

	auto events = EventQueue::instance();
	while(events->peekEvent()->type() != Events::Quit) {
		if(!events->empty()) {
			auto event = events->getEvent();
			std::cout << *event << '\n';
			event->dispatch();
		}
		events->waitForEvents();
	}
	return 0;
}