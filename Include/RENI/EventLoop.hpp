#ifndef RENI_EVENT_LOOP_HEADER
#define RENI_EVENT_LOOP_HEADER

namespace RENI {
	/**
	 * @brief Manages GUI event loops.
	 */
	class EventLoop {
	public:
		/** @{ */
		/** @brief Terminate all active event loops. */
		static void Quit(int exitCode = 0);
		/** @} */


		/** @{ */
		EventLoop() = default;
		~EventLoop() = default;
		/** @} */


		/** @{ */
		EventLoop(EventLoop&&) = default;
		EventLoop& operator=(EventLoop&&) = default;
		/** @} */

		/** @{ */
		EventLoop(const EventLoop&) = delete;
		EventLoop& operator=(const EventLoop&) = delete;
		/** @} */


		/** @{ */
		/** @brief Handle all available input system events. */
		bool HandleInput();

		/** @brief Blocks until new input is available. */
		void Wait();
		/** @} */


		/** @{ */
		/** @brief Get the event loop's termination status. */
		int GetExitCode() const;
		
		/** @brief Start a loop that continuously processes incoming input events. */
		int Run();
		/** @} */
	};
}

#endif