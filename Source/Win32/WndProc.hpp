#ifndef RENI_WND_PROC_HEADER
#define RENI_WND_PROC_HEADER

#include <Windows.h>

namespace RENI::Win32 {
	/**
	 * @brief Manages the main window procedure that drives @ref Window "Windows".
	 */
	class WndProc {
	public:		
		/** @{ */
		/** @brief Check if any exceptions were thrown the last time the window procedure was called. */
		static bool AnyExceptions() noexcept;
		/** @brief Rethrow any exceptions that were thrown during the last window procedure call. */
		static void RethrowExceptions();
		/** @} */

		/** @{ */
		/** @brief Get the number of currently visible @ref Window "Windows". */
		static int WindowsVisible() noexcept;
		/** @} */

		/** @{ */
		/** @brief Get the window procedure. */
		static WNDPROC Get() noexcept;
		/** @} */

	private:
		WndProc() = delete;
		~WndProc() = delete;
	};
}

#endif