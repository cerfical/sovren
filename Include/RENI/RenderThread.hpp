#ifndef RENI_RENDER_THREAD_HEADER
#define RENI_RENDER_THREAD_HEADER

#include <memory>

namespace RENI {
	class RenderWindow;
	
	/**
	 * @brief Performs some rendering task in a separate thread.
	 */
	class RenderThread {
	public:
		/** @{ */
		/**
		 * @brief Describes the thread operating states.
		 */
		enum Status {
			Stopped,
			Stopping,
			Running,
			Resuming,
			Suspended,
			Suspending
		};
		/** @} */


		/** @{ */
		/** @brief Construct a new rendering thread and start it immediately. */
		RenderThread(RenderWindow* targetWindow);

		/** @brief Stop and destroy the thread. */
		~RenderThread();
		/** @} */


		/** @{ */
		RenderThread(RenderThread&&) = default;
		RenderThread& operator=(RenderThread&&) = default;
		/** @} */

		/** @{ */
		RenderThread(const RenderThread&) = delete;
		RenderThread& operator=(const RenderThread&) = delete;
		/** @} */

		
		/** @{ */
		/** @brief Starts the thread if it is not already running. */
		void start();

		/** @brief Stops the thread if it is not already stopped. */
		void stop();
		/** @} */
		

		/** @{ */
		/** @brief Suspends the execution of the thread. */
		void suspend();
		
		/** @brief Resumes the execution of the thread. */
		void resume();
		/** @} */
		

		/** @{ */
		/** @brief Operating state of the thread. */
		Status status() const noexcept;

		/** @brief Check if the thread is currently running. */
		bool running() const noexcept {
			return status() == Status::Running;
		}

		/** @brief Check if the thread was suspended. */
		bool suspended() const noexcept {
			return status() == Status::Suspended;
		}

		/** @brief Check if the thread was stopped. */
		bool stopped() const noexcept {
			return status() == Status::Stopped;
		}
		/** @} */


	private:
		/** @{ */
		bool updateStatus();
		void render();
		/** @} */


		/** @{ */
		struct Impl;
		std::unique_ptr<Impl> m_impl;
		/** @} */
	};
}

#endif