#ifndef RENI_WINDOW_SYSTEM_HEADER
#define RENI_WINDOW_SYSTEM_HEADER

#include "WindowHandle.hpp"
#include "EventQueue.hpp"

namespace RENI {
	/**
	 * @brief Abstraction for the platform's underlying windowing system.
	 */
	class WindowSystem {
	public:
		/** @{ */
		/** @brief Access to the globally accessible singleton instance of the WindowSystem. */
		static WindowSystem* instance();
		/** @} */

		/** @{ */
		/** @brief Allocate a new window in the windowing system. */
		virtual WindowHandle* createWindow() = 0;
		
		/** @brief EventQueue associated with the windowing system. */
		virtual EventQueue* eventQueue() = 0;
		/** @} */

	protected:
		/** @{ */
		~WindowSystem() = default;
		/** @} */
	};
}

#endif