#ifndef RENI_WIN_UI_FACTORY_HEADER
#define RENI_WIN_UI_FACTORY_HEADER

#include "WinWindow.hpp"
#include "Canvas.hpp"

#include <Windows.h>
#include <memory>

namespace RENI::Win32 {
	/**
	 * @brief Factory to create Win32-specific UI elements.
	 */
	class WinUiFactory {
	public:
		/** @{ */
		/** @brief Properly initialize and return globally shared singleton instance of WinUiFactory. */
		static std::shared_ptr<WinUiFactory> Get();
		/** @} */

		/** @{ */
		/** @brief Destroy the WinUiFactory. */
		virtual ~WinUiFactory() = default;
		/** @} */

		/** @{ */
		WinUiFactory(const WinUiFactory&) = delete;
		WinUiFactory& operator=(const WinUiFactory&) = delete;
		/** @} */

		/** @{ */
		WinUiFactory(WinUiFactory&&) = delete;
		WinUiFactory& operator=(WinUiFactory&&) = delete;
		/** @} */

		/** @{ */
		/** @brief Create a new Canvas attached to the WinWindow. */
		virtual std::unique_ptr<Canvas> CreateCanvas(WinWindow& window) = 0;
		/** @} */

	protected:
		/** @{ */
		/** @brief Construct a new WinUiFactory. */
		WinUiFactory() = default;
		/** @} */
	};
}

#endif