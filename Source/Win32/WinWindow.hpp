#ifndef RENI_WIN_WINDOW_HEADER
#define RENI_WIN_WINDOW_HEADER

#include "Utils.hpp"
#include "WinWndClass.hpp"
#include "WinUtils.hpp"

#include <exception>
#include <memory>

namespace RENI {
	/**
	 * @brief Manages a Win32 window.
	 */
	class WinWindow {
	public:
		/** @{ */
		/** @brief Get a window associated with the given handle, if any. */
		static WinWindow* FromHandle(HWND handle);
		/** @} */


		/** @{ */
		/** @brief Initialize a new Win32 window. */
		WinWindow();

		/** @brief Destroy the window. */
		~WinWindow() = default;
		/** @} */

		/** @{ */
		WinWindow(WinWindow&&) = default;
		WinWindow& operator=(WinWindow&&) = default;
		/** @} */

		/** @{ */
		WinWindow(const WinWindow&) = delete;
		WinWindow& operator=(const WinWindow&) = delete;
		/** @} */


		/** @{ */
		void SetClientArea(const Extent2D& clientArea);
		Extent2D GetClientArea() const;

		void SetTitle(std::string_view title);
		std::string GetTitle() const;

		void SetVisible(bool visible);
		bool IsVisible() const;
		/** @} */


		/** @{ */
		Point2D GetCursorPos() const;

		void CaptureMouse();
		void ReleaseMouse();
		/** @} */


		/** @{ */
		void HandleMsg(const MSG& msg);
		HWND GetHandle() const noexcept {
			return m_handle.get();
		}
		/** @} */


	protected:
		/** @{ */
		/** @brief Call a window state changing function in an exception-safe manner. */
		template <typename C, typename... Args>
			requires std::invocable<C, Args...>
		static auto SafeWndProcCall(C&& func, Args&&... args) {
			const auto result = SafeWin32ApiCall(std::forward<C>(func), std::forward<Args>(args)...);
			if(wndProcException) {
				std::rethrow_exception(std::exchange(
					wndProcException, nullptr
				));
			}
			return result;
		}
		/** @} */

		/** @{ */
		/** @brief Called when a message sent to this window needs to be processed. */
		virtual LRESULT OnMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
		/** @} */


	private:
		/** @{ */
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		/** @} */


		/** @{ */
		static const WinWndClass* GetWndClass();
		static HWND CreateHWnd();
		/** @} */


		class HwndDeleter {
		public:
			using pointer = HWND;

			void operator()(pointer handle);
		};

		static std::exception_ptr wndProcException;
		std::unique_ptr<HWND, HwndDeleter> m_handle;
	};
}

#endif