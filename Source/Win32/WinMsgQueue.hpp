#ifndef RENI_WIN_MSG_QUEUE_HEADER
#define RENI_WIN_MSG_QUEUE_HEADER

#include <Windows.h>

namespace RENI {
	/**
	 * @brief Provides an interface to the thread's message queue.
	 */
	class WinMsgQueue {
	public:
		/** @{ */
		WinMsgQueue() = default;
		~WinMsgQueue() = default;
		/** @} */

		/** @{ */
		WinMsgQueue(WinMsgQueue&&) = default;
		WinMsgQueue& operator=(WinMsgQueue&&) = default;
		/** @} */

		/** @{ */
		WinMsgQueue(const WinMsgQueue&) = delete;
		WinMsgQueue& operator=(const WinMsgQueue&) = delete;
		/** @} */

		/** @{ */
		/** @brief Extract the next available message from the queue, waiting for one if none. */
		const MSG& GetMsg();

		/** @brief Get the last message extracted from the queue. */
		const MSG& GetLastMsg() const noexcept {
			return m_msg;
		}

		/** @brief Check if there are any messages in the queue. */
		bool IsEmpty() const;
		/** @} */

	private:
		mutable MSG m_msg = { };
	};
}

#endif