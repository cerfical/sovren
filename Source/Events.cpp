#include "Events.hpp"
#include "Window.hpp"

#define EVENT_PREFIX type() << ": "

namespace RENI {
	std::ostream& operator<<(std::ostream& out, Events event) {
#define RENI_EVENT(e) case Events::e: out << "<" #e ">"; break;

		switch(event) {
			RENI_EVENT_LIST
		}

#undef RENI_EVENT
		return out;
	}

	std::ostream& operator<<(std::ostream& out, const Event& event) {
		event.print(out);
		return out;
	}


	void Event::print(std::ostream& out) const {
		out << type();
	}

	void Event::dispatch() {
		if(m_targetWindow) {
			m_targetWindow->triggerEvent(*this);
		}
	}


	void QuitEvent::print(std::ostream& out) const {
		out << EVENT_PREFIX << exitCode();
	}


	void ResizeEvent::print(std::ostream& out) const {
		out << EVENT_PREFIX << "from " << oldSize() << " to " << newSize();
	}


	void KeyPressEvent::print(std::ostream& out) const {
		out << EVENT_PREFIX << pressedKey();
	}


	void KeyReleaseEvent::print(std::ostream& out) const {
		out << EVENT_PREFIX << releasedKey();
	}


	void MousePressEvent::print(std::ostream& out) const {
		out << EVENT_PREFIX << pressedButton();
	}


	void MouseReleaseEvent::print(std::ostream& out) const {
		out << EVENT_PREFIX << releasedButton();
	}


	void MouseMoveEvent::print(std::ostream& out) const {
		out << EVENT_PREFIX << "from " << oldPos() << " to " << newPos();
	}
}