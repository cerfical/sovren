#ifndef RENI_GUI_APP_HEADER
#define RENI_GUI_APP_HEADER

#include "Utils.hpp"
#include "WindowSystem.hpp"

namespace RENI {
	/**
	 * @brief Manages the GUI application currently being executed.
	 */
	class GuiApp {
	public:
		/** @{ */
		/** @brief Initialize a new GUI application. */
		GuiApp() = default;

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
		int exec();
		/** @} */

		/** @{ */
		/** @brief Windowing system for the GUI application. */
		WindowSystem* windowSystem() const {
			return WindowSystem::instance();
		}

		/** @brief Event queue for managing GUI events. */
		EventQueue* events() const {
			return windowSystem()->eventQueue();
		}
		/** @} */

	protected:
		/** @{ */
		/** @brief Called during application execution. */
		virtual void onExec();
		/** @} */
	};
}

#endif