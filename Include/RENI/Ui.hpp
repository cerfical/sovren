#ifndef RENI_UI_HEADER
#define RENI_UI_HEADER

namespace RENI {
	/**
	 * @brief User interface management.
	 */
	class Ui {
	public:
		/** @{ */
		/** @brief Continuously poll the system for new events and, if any, dispatch them to the appropriate destinations. */
		static int EnterEventLoop();
		/** @} */

	private:
		Ui() = delete;
		~Ui() = delete;
	};
}

#endif