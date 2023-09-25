#ifndef RENI_MESSAGE_QUEUE_HEADER
#define RENI_MESSAGE_QUEUE_HEADER

#include <Windows.h>

namespace RENI {
	/**
	 * @brief Communicates with the current thread's message queue.
	 */
	class MessageQueue {
	public:
		/** @{ */
		void DispatchMessages();
		void Wait() const;
		/** @} */

		/** @{ */
		void PostQuitMessage(int exitCode);
		bool IsQuitMessage() const noexcept;

		int GetExitCode() const noexcept;
		/** @} */

	private:
		MSG m_msg = { };
	};
}

#endif