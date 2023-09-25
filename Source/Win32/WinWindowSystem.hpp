#ifndef RENI_WIN_WINDOW_SYSTEM_HEADER
#define RENI_WIN_WINDOW_SYSTEM_HEADER

#include "WindowSystem.hpp"
#include <Windows.h>

namespace RENI {
	/**
	 * @brief Win32 windowing system.
	 */
	class WinWindowSystem : public WindowSystem {
	public:
		/** @{ */
		static WinWindowSystem* instance() noexcept;
		/** @} */

		/** @{ */
		WindowHandle* createWindow() override;
		EventQueue* eventQueue() override;
		/** @} */

		/** @{ */
		Window* windowFromHandle(HWND handle);
		/** @} */

	private:
		/** @{ */
		WinWindowSystem() = default;
		~WinWindowSystem() = default;
		/** @} */

		/** @{ */
		WinWindowSystem(WinWindowSystem&&) = delete;
		WinWindowSystem& operator=(WinWindowSystem&&) = delete;
		/** @} */

		/** @{ */
		WinWindowSystem(const WinWindowSystem&) = delete;
		WinWindowSystem& operator=(const WinWindowSystem&) = delete;
		/** @} */
	};
}

#endif