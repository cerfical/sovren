#ifndef RENI_WIN_WINDOW_HEADER
#define RENI_WIN_WINDOW_HEADER

#include "WindowHandle.hpp"
#include "WinWndClass.hpp"

#include <Windows.h>
#include <memory>

namespace RENI {
	/**
	 * @brief Manages Win32-specific window handles.
	 */
	class WinWindow : public WindowHandle {
	public:
		/** @{ */
		WinWindow();
		/** @} */

		/** @{ */
		void setClientSize(const Size2D& size) override;
		Size2D clientSize() const override;

		void setTitle(std::string_view title) override;
		std::string title() const override;

		void setVisible(bool visible) override;
		bool visible() const override;
		/** @} */

		/** @{ */
		void setMouseCapture(bool enable) override;
		Point2D mousePos() const override;
		/** @} */

		/** @{ */
		void* nativeHandle() const override {
			return reinterpret_cast<void*>(handle());
		}
		HWND handle() const noexcept {
			return m_handle.get();
		}
		/** @} */

	private:
		/** @{ */
		static std::shared_ptr<WinWndClass> wndClass();
		static HWND createHwnd(const WinWndClass& wndClass);
		/** @} */

		class HwndDeleter {
		public:
			using pointer = HWND;
			void operator()(pointer handle);
		};

		std::shared_ptr<WinWndClass> m_wndClass;
		std::unique_ptr<HWND, HwndDeleter> m_handle;
	};
}

#endif