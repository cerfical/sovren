#ifndef RENI_WIN_WINDOW_HEADER
#define RENI_WIN_WINDOW_HEADER

#include "Utils.hpp"

#include "MouseButtons.hpp"
#include "Keys.hpp"

#include "WndClass.hpp"

namespace RENI {
	/**
	 * @brief Manages Win32-native windows.
	 */
	class WinWindow {
	public:
		/** @{ */
		static WinWindow* FromHandle(HWND handle);
		/** @} */

		/** @{ */
		WinWindow();
		/** @} */

		/** @{ */
		void SetClientSize(const Size2D& size);
		Size2D GetClientSize() const;

		void SetTitle(std::string_view title);
		std::string GetTitle() const;

		void SetVisible(bool visible);
		bool IsVisible() const;
		/** @} */

		/** @{ */
		void ToggleMouseCapture();
		Point2D GetCursorPos() const;
		/** @} */

		/** @{ */
		HWND GetHandle() const noexcept {
			return m_handle.get();
		}
		/** @} */

	protected:
		/** @{ */
		virtual void OnResize(const Size2D& newSize) { }
		virtual void OnClose() { }
		/** @} */

		/** @{ */
		virtual void OnKeyPress(Keys pressedKey) { }
		virtual void OnKeyRelease(Keys releasedKey) { }
		/** @} */

		/** @{ */
		virtual void OnMousePress(MouseButtons pressedButton) { }
		virtual void OnMouseRelease(MouseButtons releasedButton) { }

		virtual void OnMouseMove(const Point2D& newPos) { }
		/** @} */

	private:
		/** @{ */
		static LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		/** @} */

		class HwndDeleter {
		public:
			using pointer = HWND;
			void operator()(pointer handle);
		};

		std::shared_ptr<WndClass> m_wndClass;
		std::unique_ptr<HWND, HwndDeleter> m_handle;
	};
}

#endif