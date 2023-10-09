#ifndef RENI_UI_MAIN_LOOP_HEADER
#define RENI_UI_MAIN_LOOP_HEADER

#include <memory>

namespace RENI {
	/**
	 * @brief Implements the UI main loop for polling system events.
	 */
	class UiMainLoop {
	public:
		/** @{ */
		/** @brief Global instance of an event loop. */
		static UiMainLoop* get();
		/** @} */


		/** @{ */
		UiMainLoop(UiMainLoop&&) = delete;
		UiMainLoop& operator=(UiMainLoop&&) = delete;
		/** @} */

		/** @{ */
		UiMainLoop(const UiMainLoop&) = delete;
		UiMainLoop& operator=(const UiMainLoop&) = delete;
		/** @} */


		/** @{ */
		/** @brief Check if the loop is running and there is no request to end it. */
		bool active() const;

		/** @brief Examines the system event queue to determine if there are events that need to be processed. */
		bool anyEvents() const;
		
		/** @brief If the loop has finished execution, reflects its completion status code. */
		int exitCode() const;
		/** @} */


		/** @{ */
		/** @brief Blocks until new events arrive. */
		void waitEvents() const;

		/** @brief Removes and processes all available system events. */
		void pollEvents();
		/** @} */


		/** @{ */
		/** @brief Terminates the loop with the specified status code. */
		void exit(int code = 0);

		/** @brief Run the loop. */
		int exec();
		/** @} */

	private:
		/** @{ */
		UiMainLoop();
		~UiMainLoop();
		/** @} */

		/** @{ */
		struct Impl;
		std::unique_ptr<Impl> m_impl;
		/** @} */
	};
}

#endif