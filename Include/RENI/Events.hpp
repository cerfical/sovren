#ifndef RENI_EVENTS_HEADER
#define RENI_EVENTS_HEADER

#define RENI_EVENT_LIST \
	RENI_EVENT(None) /**< @brief Empty nil event. */ \
	RENI_EVENT(Resize) /**< @brief Window resize. */ \
	RENI_EVENT(Close) /**< @brief Window close. */ \
	\
	RENI_EVENT(KeyPress) /**< @brief Key press. */ \
	RENI_EVENT(KeyRelease) /**< @brief Key release. */ \
	\
	RENI_EVENT(MousePress) /**< @brief Mouse button press. */ \
	RENI_EVENT(MouseRelease) /**< @brief Mouse button release. */ \
	RENI_EVENT(MouseMove) /**< @brief Mouse cursor move. */ \
	\
	RENI_EVENT(Quit) /**< @brief Application quit. */

#include "Utils.hpp"
#include "MouseButtons.hpp"
#include "Keys.hpp"

#include <iostream>

namespace RENI {

#define RENI_EVENT(e) e,

	/**
	 * @brief Enumerates available GUI events.
	 */
	enum class Events {
		RENI_EVENT_LIST
	};

#undef RENI_EVENT

	/** @brief Print an Events enumerator to a @c std::ostream. */
	std::ostream& operator<<(std::ostream& out, Events event);



	class Window;
	/**
	 * @brief Notification of a visible state change in the GUI.
	 */
	class Event {
	public:
		/** @brief Print out an event to a @c std::ostream. */
		friend std::ostream& operator<<(std::ostream& out, const Event& event);


		/** @{ */
		/** @brief Check if the event is the nil event. */
		operator bool() const noexcept {
			return type() != Events::None;
		}
		/** @} */


		/** @{ */
		/** @brief Print out the event to a @c std::ostream. */
		virtual void print(std::ostream& out = std::cout) const;
		
		/** @brief Type of the event. */
		virtual Events type() const noexcept {
			return Events::None;
		}
		/** @} */
		

		/** @{ */
		/** @brief Send the event to its target window. */
		void dispatch();

		/** @brief Target window for the event. */
		Window* targetWindow() const noexcept {
			return m_targetWindow;
		}
		/** @} */


		/** @{ */
		/** @brief Present the event interface as of the specified type if possible. */
		template <typename EventType>
		EventType* as() {
			return dynamic_cast<EventType*>(this);
		}

		/** @brief Present the event interface as of the specified type if possible. */
		template <typename EventType>
		const EventType* as() const {
			return dynamic_cast<const EventType*>(this);
		}
		/** @} */

	protected:
		/** @{ */
		/** @brief Initialize the event with the target window. */
		explicit Event(Window* targetWindow = nullptr) noexcept
			: m_targetWindow(targetWindow)
		{ }

		~Event() = default;
		/** @} */
		
		/** @{ */
		Event(const Event&) = default;
		Event& operator=(const Event&) = default;
		/** @} */

	private:
		Window* m_targetWindow = nullptr;
	};



	/**
	 * @brief Empty event.
	 */
	class NilEvent : public Event {
	public:
		/** @{ */
		NilEvent() = default;
		/** @} */

		/** @{ */
		NilEvent(const NilEvent&) = default;
		NilEvent& operator=(const NilEvent&) = default;
		/** @} */
	};



	/**
	 * @brief Represents an intent to exit a GUI application.
	 */
	class QuitEvent : public Event {
	public:
		/** @{ */
		/** @brief Construct a new QuitEvent initialized with an exit code. */
		explicit QuitEvent(int exitCode = 0) noexcept
			: m_exitCode(exitCode)
		{ }
		/** @} */

		/** @{ */
		QuitEvent(const QuitEvent&) = default;
		QuitEvent& operator=(const QuitEvent&) = default;
		/** @} */

		/** @{ */
		void print(std::ostream& out) const override;

		Events type() const noexcept override {
			return Events::Quit;
		}
		/** @} */

		/** @{ */
		/** @brief Value for the exit code. */
		int exitCode() const noexcept {
			return m_exitCode;
		}
		/** @} */

	private:
		int m_exitCode = 0;
	};



	/**
	 * @brief Window resize event.
	 */
	class ResizeEvent : public Event {
	public:
		/** @{ */
		/** @brief Construct a new ResizeEvent. */
		ResizeEvent(const Size2D& newSize, const Size2D& oldSize, Window* targetWindow) noexcept
			: Event(targetWindow), m_newSize(newSize), m_oldSize(oldSize)
		{ }
		/** @} */

		/** @{ */
		ResizeEvent(const ResizeEvent&) = default;
		ResizeEvent& operator=(const ResizeEvent&) = default;
		/** @} */

		/** @{ */
		void print(std::ostream& out) const override;

		Events type() const noexcept override {
			return Events::Resize;
		}
		/** @} */

		/** @{ */
		/** @brief New size of the window. */
		const Size2D& newSize() const noexcept {
			return m_newSize;
		}

		/** @brief Previous size of the window. */
		const Size2D& oldSize() const noexcept {
			return m_oldSize;
		}
		/** @} */

	private:
		Size2D m_newSize;
		Size2D m_oldSize;
	};



	/**
	 * @brief Window close event.
	 */
	class CloseEvent : public Event {
	public:
		/** @{ */
		/** @brief Construct a new CloseEvent. */
		explicit CloseEvent(Window* targetWindow) noexcept
			: Event(targetWindow)
		{ }
		/** @} */

		/** @{ */
		CloseEvent(const CloseEvent&) = default;
		CloseEvent& operator=(const CloseEvent&) = default;
		/** @} */

		/** @{ */
		Events type() const noexcept override {
			return Events::Close;
		}
		/** @} */
	};



	/**
	 * @brief Key press event.
	 */
	class KeyPressEvent : public Event {
	public:
		/** @{ */
		/** @brief Construct a new KeyPressEvent. */
		KeyPressEvent(Keys pressedKey, Window* targetWindow) noexcept
			: Event(targetWindow), m_pressedKey(pressedKey)
		{ }
		/** @} */

		/** @{ */
		KeyPressEvent(const KeyPressEvent&) = default;
		KeyPressEvent& operator=(const KeyPressEvent&) = default;
		/** @} */

		/** @{ */
		/** @brief Check if the specified key was pressed. */
		bool operator[](Keys k) const noexcept {
			return m_pressedKey == k;
		}
		/** @} */

		/** @{ */
		void print(std::ostream& out) const override;
		
		Events type() const noexcept override {
			return Events::KeyPress;
		}
		/** @} */

		/** @{ */
		/** @brief Key that was pressed. */
		Keys pressedKey() const noexcept {
			return m_pressedKey;
		}
		/** @} */

	private:
		Keys m_pressedKey;
	};



	/**
	 * @brief Key release event.
	 */
	class KeyReleaseEvent : public Event {
	public:
		/** @{ */
		/** @brief Construct a new KeyReleaseEvent. */
		KeyReleaseEvent(Keys releasedKey, Window* targetWindow) noexcept
			: Event(targetWindow), m_releasedKey(releasedKey)
		{ }
		/** @} */

		/** @{ */
		KeyReleaseEvent(const KeyReleaseEvent&) = default;
		KeyReleaseEvent& operator=(const KeyReleaseEvent&) = default;
		/** @} */

		/** @{ */
		/** @brief Check if the specified key was released. */
		bool operator[](Keys k) const noexcept {
			return m_releasedKey == k;
		}
		/** @} */

		/** @{ */
		void print(std::ostream& out) const override;
		
		Events type() const noexcept override {
			return Events::KeyRelease;
		}
		/** @} */

		/** @{ */
		/** @brief Key that was released. */
		Keys releasedKey() const noexcept {
			return m_releasedKey;
		}
		/** @} */

	private:
		Keys m_releasedKey;
	};



	/**
	 * @brief Mouse press event.
	 */
	class MousePressEvent : public Event {
	public:
		/** @{ */
		/** @brief Construct a new MousePressEvent. */
		MousePressEvent(MouseButtons pressedButton, Window* targetWindow) noexcept
			: Event(targetWindow), m_pressedButton(pressedButton)
		{ }
		/** @} */

		/** @{ */
		MousePressEvent(const MousePressEvent&) = default;
		MousePressEvent& operator=(const MousePressEvent&) = default;
		/** @} */

		/** @{ */
		/** @brief Check if the specified button was pressed. */
		bool operator[](MouseButtons b) const noexcept {
			return m_pressedButton == b;
		}
		/** @} */

		/** @{ */
		void print(std::ostream& out) const override;
		
		Events type() const noexcept override {
			return Events::MousePress;
		}
		/** @} */

		/** @{ */
		/** @brief Button that was pressed. */
		MouseButtons pressedButton() const noexcept {
			return m_pressedButton;
		}
		/** @} */

	private:
		MouseButtons m_pressedButton;
	};



	/**
	 * @brief Mouse release event.
	 */
	class MouseReleaseEvent : public Event {
	public:
		/** @{ */
		/** @brief Construct a new MouseReleaseEvent. */
		MouseReleaseEvent(MouseButtons releasedButton, Window* targetWindow) noexcept
			: Event(targetWindow), m_releasedButton(releasedButton)
		{ }
		/** @} */

		/** @{ */
		MouseReleaseEvent(const MouseReleaseEvent&) = default;
		MouseReleaseEvent& operator=(const MouseReleaseEvent&) = default;
		/** @} */

		/** @{ */
		/** @brief Check if the specified button was released. */
		bool operator[](MouseButtons b) const noexcept {
			return m_releasedButton == b;
		}
		/** @} */

		/** @{ */
		void print(std::ostream& out) const override;
		
		Events type() const noexcept override {
			return Events::MouseRelease;
		}
		/** @} */

		/** @{ */
		/** @brief Button that was released. */
		MouseButtons releasedButton() const noexcept {
			return m_releasedButton;
		}
		/** @} */

	private:
		MouseButtons m_releasedButton;
	};



	/**
	 * @brief Mouse move event.
	 */
	class MouseMoveEvent : public Event {
	public:
		/** @{ */
		/** @brief Construct a new MouseMoveEvent. */
		MouseMoveEvent(const Point2D& newPos, const Point2D& oldPos, Window* targetWindow) noexcept
			: Event(targetWindow), m_newPos(newPos), m_oldPos(oldPos)
		{ }
		/** @} */

		/** @{ */
		MouseMoveEvent(const MouseMoveEvent&) = default;
		MouseMoveEvent& operator=(const MouseMoveEvent&) = default;
		/** @} */

		/** @{ */
		void print(std::ostream& out) const override;
		
		Events type() const noexcept override {
			return Events::MouseMove;
		}
		/** @} */

		/** @{ */
		/** @brief New cursor position of the mouse. */
		const Point2D& newPos() const noexcept {
			return m_newPos;
		}
		
		/** @brief Previous cursor position of the mouse. */
		const Point2D& oldPos() const noexcept {
			return m_oldPos;
		}

		/** @brief Mouse cursor displacement. */
		Offset2D displace() const noexcept {
			return newPos() - oldPos();
		}
		/** @} */

	private:
		Point2D m_newPos;
		Point2D m_oldPos;
	};



	/**
	 * @brief Interface for monitoring keyboard events.
	 */
	class KeysObserver {
	public:
		/** @{ */
		/** @brief Called when a key is pressed. */
		virtual void onKeyPress(const KeyPressEvent& e) { }

		/** @brief Called when a key is released. */
		virtual void onKeyRelease(const KeyReleaseEvent& e) { }
		/** @} */

	protected:
		/** @{ */
		~KeysObserver() = default;
		/** @} */
	};



	/**
	 * @brief Interface for monitoring mouse events.
	 */
	class MouseObserver {
	public:
		/** @{ */
		/** @brief Called when a mouse button is pressed. */
		virtual void onMousePress(const MousePressEvent& e) { }

		/** @brief Called when a mouse button is released. */
		virtual void onMouseRelease(const MouseReleaseEvent& e) { }

		/** @brief Called when the mouse is moved. */
		virtual void onMouseMove(const MouseMoveEvent& e) { }
		/** @} */

	protected:
		/** @{ */
		~MouseObserver() = default;
		/** @} */
	};
}

#endif