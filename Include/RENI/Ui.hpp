#ifndef RENI_UI_HEADER
#define RENI_UI_HEADER

namespace RENI {
	/**
	 * @brief UI management.
	 */
	class Ui {
	public:
		/** @{ */
		/** @brief Start a loop that will continuously dispatch events to appropriate windows. */
		static int EnterUiLoop();
		/** @} */

	private:
		Ui() = delete;
		~Ui() = delete;
	};
}

#endif