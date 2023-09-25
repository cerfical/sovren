#ifndef RENI_EVENTS_HEADER
#define RENI_EVENTS_HEADER

#include "Utils.hpp"

namespace RENI {
	/**
	 * @brief Events that a Window can generate.
	 */
	enum class EventTypes {
		MouseMove, /**< @brief Mouse movement was detected. */
	
		WindowShow, /**< @brief Window was made visible. */
		WindowHide, /**< @brief Window was hidden. */

		WindowClose, /**< @brief Window was closed. */
		WindowResize, /**< @brief Window was resized. */
		WindowDraw /**< @brief Window needs to draw itself. */
	};

	/**
	 * @brief Interface for handling Window events.
	 */
	class EventObserver {
	public:
		/** @{ */
		/** @brief Destroy the EventObserver. */
		virtual ~EventObserver() = 0;
		/** @} */

		/** @{ */
		/** @copybrief EventTypes::MouseMove. */
		virtual void OnMouseMove() { }
		
		/** @copybrief EventTypes::WindowShow. */
		virtual void OnWindowShow() { }
		/** @copybrief EventTypes::WindowHide. */
		virtual void OnWindowHide() { }

		/** @copybrief EventTypes::WindowClose. */
		virtual void OnWindowClose() { }

		/** @copybrief EventTypes::WindowResize. */
		virtual void OnWindowResize() { }
		/** @copybrief EventTypes::WindowDraw. */
		virtual void OnWindowDraw() { }
		/** @} */

	protected:
		/** @{ */
		/** @brief Construct a new EventObserver. */
		EventObserver() = default;
		/** @} */
	};

	inline EventObserver::~EventObserver() = default;
}

#endif