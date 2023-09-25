#ifndef RENI_EVENT_QUEUE_HEADER
#define RENI_EVENT_QUEUE_HEADER

#include "Events.hpp"

namespace RENI {
	/**
	 * @brief Abstraction for retrieving events from the system.
	 */
	class EventQueue {
	public:
		/** @{ */
		/** @brief Event queue for the underlying windowing system. */
		static EventQueue* instance();
		/** @} */


		/** @{ */
		/** @brief Put a QuitEvent to the queue. */
		virtual void postQuitEvent(int exitCode = 0) = 0;

		/** @brief Block the calling site until there is at least one event in the queue. */
		virtual void waitForEvents() const = 0;
		
		/** @brief Check if there are any events in the queue. */
		bool empty() const {
			return !*peekEvent();
		}
		/** @} */


		/** @{ */
		/** @brief Extract the next event from the queue. */
		virtual Event* getEvent() = 0;

		/** @brief Look at the next event in the queue. */
		virtual Event* peekEvent() const = 0;
		/** @} */

	protected:
		/** @{ */
		~EventQueue() = default;
		/** @} */
	};
}

#endif