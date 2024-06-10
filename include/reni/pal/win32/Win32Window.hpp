#pragma once

#include "../Window.hpp"
#include "../WindowCallbacks.hpp"

#include "WndClass.hpp"
#include "util.hpp"

#include <optional>
#include <Windows.h> 

namespace reni::pal::win32 {

	class Win32Window : public Window {
	public:

		Win32Window();


		void installCallbacks(WindowCallbacks* callbacks) override {
			m_callbacks = callbacks;
		}


		void setTitle(std::string_view newTitle) override;


		void setClientSize(Size2 newSize) override;


		void setVisible(bool visible) override {
			win32Check(ShowWindow(m_handle.get(), visible ? SW_SHOW : SW_HIDE));
		}


		std::string getTitle() const override;


		Size2 getClientSize() const override;


		bool isVisible() const override {
			return static_cast<bool>(win32Check(IsWindowVisible(m_handle.get())));
		}


		Point2 getMousePos() const override;


		void* nativeHandle() const override {
			return m_handle.get();
		}


	private:
		static LRESULT wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

		static std::optional<Key> mapVkeyToKeys(WPARAM vkey) noexcept;

		static Win32Window* fromHandle(HWND handle);


		template <typename... Args, typename... Types>
			requires (std::convertible_to<Args, Types> && ...)
		LRESULT performCallback(void (WindowCallbacks::* callback)(Types...), Args&&... args) {
			if(m_callbacks) {
				(m_callbacks->*callback)(std::forward<Args>(args)...);
			}
			return 0;
		}


		class HwndDeleter {
		public:
			using pointer = HWND;
			
			void operator()(pointer handle) {
				win32Check(DestroyWindow(handle));
			}
		};

		std::shared_ptr<WndClass> m_wndClass;
		std::unique_ptr<HWND, HwndDeleter> m_handle;
		WindowCallbacks* m_callbacks = {};
	};

}