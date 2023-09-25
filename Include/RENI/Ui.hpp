#ifndef RENI_UI_HEADER
#define RENI_UI_HEADER

namespace RENI {
	/**
	 * @brief User interface management.
	 */
	class Ui {
	public:
		/** @{ */
		/** @brief Enter the GUI event loop. */
		static int eventLoop();
		/** @} */

	private:
		/** @{ */
		Ui() = delete;
		~Ui() = delete;
		/** @} */
	};
}

#endif