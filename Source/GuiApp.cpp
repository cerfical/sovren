#include "GuiApp.hpp"

namespace RENI {
	int GuiApp::exec() {
		onExec();
		if(events()->peekEvent()->type() == Events::Quit) {
			return events()->peekEvent()->as<QuitEvent>()->exitCode();
		}
		return 0;
	}

	void GuiApp::onExec() {
		while(events()->peekEvent()->type() != Events::Quit) {
			if(!events()->empty()) {
				auto event = events()->getEvent();
				event->dispatch();
			}
			events()->waitForEvents();
		}
	}
}