#ifndef RENI_WIN_EVENT_QUEUE_HEADER
#define RENI_WIN_EVENT_QUEUE_HEADER

#include "EventQueue.hpp"
#include <Windows.h>

#include <optional>
#include <memory>

namespace RENI {
	/**
	 * @brief Win32 implementation of EventQueue interface for handling system messages.
	 */
	class WinEventQueue : public EventQueue {
	public:
		/** @{ */
		static LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		static WinEventQueue* instance();
		/** @} */

		/** @{ */
		void postQuitEvent(int exitCode) override;
		void waitForEvents() const override;
		/** @} */

		/** @{ */
		Event* getEvent() override;
		Event* peekEvent() const override;
		/** @} */

	private:
		/** @{ */
		static NilEvent nilEvent;
		/** @} */

		/** @{ */
		static std::optional<Keys> mapVKeyToKey(WPARAM vkey) noexcept;
		static void WINAPI msgHandler(LPVOID) noexcept;
		/** @} */


		/** @{ */
		WinEventQueue();
		~WinEventQueue() = default;
		/** @} */

		/** @{ */
		WinEventQueue(WinEventQueue&&) = delete;
		WinEventQueue& operator=(WinEventQueue&&) = delete;
		/** @} */

		/** @{ */
		WinEventQueue(const WinEventQueue&) = delete;
		WinEventQueue& operator=(const WinEventQueue&) = delete;
		/** @} */

		
		/** @{ */
		bool insideMsgHandler() const;

		void yieldEvent(const Event& event);
		void yieldNoEvents();
		/** @} */


		/** @{ */
		struct ThreadFiberDeleter {
			using pointer = LPVOID;
			void operator()(pointer fiber) const;
		};

		struct FiberDeleter {
			using pointer = LPVOID;
			void operator()(pointer fiber) const;
		};
		/** @} */


		std::unique_ptr<LPVOID, ThreadFiberDeleter> m_mainFiber;
		std::unique_ptr<LPVOID, FiberDeleter> m_msgHandlerFiber;

		Event* m_event = &nilEvent;
	};
}

#endif