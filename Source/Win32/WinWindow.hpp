#ifndef RENI_WIN_WINDOW_HEADER
#define RENI_WIN_WINDOW_HEADER

#include "Utils.hpp"
#include "WndClass.hpp"

#include <Windows.h>
#include <memory>

namespace RENI::Win32 {
	/**
	 * @brief Manages Win32 window resources.
	 */
	class WinWindow {
	public:
		/** @{ */
		/** @brief Get a WinWindow object associated with the given window handle, if any. */
		static WinWindow* FromHandle(HWND handle);
		/** @} */

		/** @{ */
		/** @brief Construct a new WinWindow. */
		WinWindow();
		/** @brief Destroy the WinWindow. */
		~WinWindow() = default;
		/** @} */

		/** @{ */
		WinWindow(const WinWindow&) = delete;
		WinWindow& operator=(const WinWindow&) = delete;
		/** @} */

		/** @{ */
		WinWindow(WinWindow&&) = delete;
		WinWindow& operator=(WinWindow&&) = delete;
		/** @} */

		/** @{ */
		/** @brief Process the messages sent to the WinWindow. */
		virtual LRESULT HandleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
		/** @} */

		/** @{ */
		void SetClientArea(Extent2D clientArea);
		Extent2D GetClientArea() const;

		void SetTitle(std::string_view title);
		std::string GetTitle() const;

		void SetVisible(bool visible);
		bool IsVisible() const;

		void SetMouseCapture();
		void ReleaseMouseCapture();

		HWND GetHandle() const noexcept {
			return handle.get();
		}
		/** @} */

	private:
		/**
		 * @brief @c std::unique_ptr<> deleter that properly destroys the window.
		 */
		class HwndDeleter {
		public:
			using pointer = HWND;

			void operator()(pointer handle);
		};

		std::shared_ptr<WndClass> wndClass;
		std::unique_ptr<HWND, HwndDeleter> handle;
	};
}

#endif