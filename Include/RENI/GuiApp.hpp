#ifndef RENI_GUI_APP_HEADER
#define RENI_GUI_APP_HEADER

#include "Utils.hpp"

namespace RENI {
	/**
	 * @brief Manages the GUI application currently being executed.
	 */
	class GuiApp {
		class Impl;

	public:
		/** @{ */
		/** @brief Initialize a new GUI application. */
		GuiApp();

		/** @brief Deinitialize the GUI application, releasing all used resources. */
		virtual ~GuiApp() = default;
		/** @} */

		/** @{ */
		GuiApp(GuiApp&&) = default;
		GuiApp& operator=(GuiApp&&) = default;
		/** @} */

		/** @{ */
		GuiApp(const GuiApp&) = delete;
		GuiApp& operator=(const GuiApp&) = delete;
		/** @} */

		/** @{ */
		/** @brief Start executing the application. */
		int Exec();
		/** @} */

	private:
		ImplHolder<Impl> m_impl;
	};
}

#endif