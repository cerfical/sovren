#ifndef RENI_GUI_APP_HEADER
#define RENI_GUI_APP_HEADER

#include "Utils.hpp"

namespace RENI {
	class GuiApp {
	public:
		/** @{ */
		GuiApp() = default;
		~GuiApp() = default;
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
		int Exec();
		/** @} */
	};
}

#endif