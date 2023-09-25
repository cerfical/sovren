#include "Window.hpp"

#include "WinUiFactory.hpp"
#include "WinWindow.hpp"
#include "WinUtils.hpp"

#include <optional>
#include <Windowsx.h>

#define RENI_VKEY_MAPPING_LIST \
	RENI_VKEY_MAPPING(VK_LEFT, LeftArrow) \
	RENI_VKEY_MAPPING(VK_RIGHT, RightArrow) \
	RENI_VKEY_MAPPING(VK_UP, UpArrow) \
	RENI_VKEY_MAPPING(VK_DOWN, DownArrow)

namespace {
	using namespace RENI;

	std::optional<Keys> VKeyToKey(WPARAM vkey) noexcept {
#define RENI_VKEY_MAPPING(vkey, key) case vkey: { return Keys::key; }
		switch(vkey) {
			RENI_VKEY_MAPPING_LIST
			default: { return { }; }
		}
#undef RENI_VKEY_MAPPING
	}
}

#undef RENI_VKEY_MAPPING_LIST

namespace RENI {
	using namespace Win32;
		
	class Window::Impl : public WinWindow {
	public:
		/** @{ */
		/** @brief Construct a new Impl object. */
		Impl() {
			canvas = WinUiFactory::Get()->CreateCanvas(*this);
			mouseState.SetCursorPos(GetCursorPos());
		}

		/** @brief Destroy the Impl object. */
		~Impl() = default;
		/** @} */


		/** @{ */
		Impl(const Impl&) = delete;
		Impl& operator=(const Impl&) = delete;
		/** @} */


		/** @{ */
		Impl(Impl&&) = delete;
		Impl& operator=(Impl&&) = delete;
		/** @} */

		std::unique_ptr<Canvas> canvas;

		mutable ObserverList<WindowObserver> observers;

		MouseState mouseState;
		KeysState keysState;

		Extent2D clientArea = { };
		bool visible = false;

	private:
		/** @{ */
		LRESULT HandleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) override {
			switch(msg) {
				// mouse messages
				case WM_LBUTTONDOWN: { OnWmLButtonDown(); break; }
				case WM_LBUTTONUP: { OnWmLButtonUp(); break; }
				case WM_MBUTTONDOWN: { OnWmMButtonDown(); break; }
				case WM_MBUTTONUP: { OnWmMButtonUp(); break; }
				case WM_RBUTTONDOWN: { OnWmRButtonDown(); break; }
				case WM_RBUTTONUP: { OnWmRButtonUp(); break; }
				case WM_MOUSEMOVE: { OnWmMouseMove({ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) }); break; }
				// keyboard messages
				case WM_KEYDOWN: {
					const auto vkey = wParam;
					if(const auto key = VKeyToKey(vkey); key) {
						OnWmKeyDown(*key);
					}
					break;
				}
				case WM_KEYUP: {
					const auto vkey = wParam;
					if(const auto key = VKeyToKey(vkey); key) {
						OnWmKeyUp(*key);
					}
					break;
				}
				// window messages
				case WM_PAINT: { OnWmPaint(); break; }
				case WM_SIZE: { OnWmSize({ LOWORD(lParam), HIWORD(lParam) }); break; }
				case WM_SHOWWINDOW: { OnWmShowWindow(wParam == TRUE); break; }
				case WM_CLOSE: { OnWmClose(); break; }
				default: {
					return WinWindow::HandleMessage(hwnd, msg, wParam, lParam);
				}
			}
			return 0;
		}
		/** @} */


		/** @{ */
		/** @brief WM_LBUTTONDOWN message. */
		void OnWmLButtonDown() {
			mouseState.PressButton(MouseButtons::Left);
		}

		/** @brief WM_LBUTTONUP message. */
		void OnWmLButtonUp() {
			mouseState.ReleaseButton(MouseButtons::Left);
		}
		

		/** @brief WM_MBUTTONDOWN message. */
		void OnWmMButtonDown() {
			mouseState.PressButton(MouseButtons::Middle);
		}

		/** @brief WM_MBUTTONUP message. */
		void OnWmMButtonUp() {
			mouseState.ReleaseButton(MouseButtons::Middle);
		}
		

		/** @brief WM_RBUTTONDOWN message. */
		void OnWmRButtonDown() {
			mouseState.PressButton(MouseButtons::Right);
		}
		
		/** @brief WM_RBUTTONUP message. */
		void OnWmRButtonUp() {
			mouseState.ReleaseButton(MouseButtons::Right);
		}


		/** @brief WM_MOUSEMOVE message. */
		void OnWmMouseMove(const Point2D& cursorPos) {
			mouseState.SetCursorPos(cursorPos);
		}


		/** @brief WM_KEYDOWN message. */
		void OnWmKeyDown(Keys key) {
			keysState.PressKey(key);
		}

		/** @brief WM_KEYUP message. */
		void OnWmKeyUp(Keys key) {
			keysState.ReleaseKey(key);
		}


		/** @brief WM_SIZE message. */
		void OnWmSize(const Extent2D& clientArea) {
			const auto oldClientArea = std::exchange(this->clientArea, clientArea);
			if(oldClientArea != clientArea) {
				canvas->Resize(clientArea);
				observers.TriggerEvent(&WindowObserver::OnWindowResize);
			}
		}

		/** @brief WM_PAINT message. */
		void OnWmPaint() {
			SafeWin32ApiCall(ValidateRect, GetHandle(), nullptr);
			observers.TriggerEvent(&WindowObserver::OnWindowDraw);
		}


		/** @brief WM_SHOWWINDOW message. */
		void OnWmShowWindow(bool visible) {
			if(this->visible != visible) {
				this->visible = visible;
				observers.TriggerEvent(visible ? &WindowObserver::OnWindowShow : &WindowObserver::OnWindowHide);
			}
		}
		
		/** @brief WM_CLOSE message. */
		void OnWmClose() {
			SetVisible(false);
			observers.TriggerEvent(&WindowObserver::OnWindowClose);
		}
		/** @} */
	};
}

namespace RENI {
	const Window::Impl& Window::GetImpl() const {
		if(!impl) {
			impl = std::make_unique<Impl>();
		}
		return static_cast<Impl&>(*impl);
	}
	
	Window::Impl& Window::GetImpl() {
		return const_cast<Impl&>(
			static_cast<const Window*>(this)->GetImpl()
		);
	}


	Window::Window() noexcept = default;
	Window::~Window() = default;


	Window::Window(Window&&) noexcept = default;
	Window& Window::operator=(Window&&) noexcept = default;


	void Window::SetClientArea(Extent2D clientArea) {
		GetImpl().SetClientArea(clientArea);
	}
	
	Extent2D Window::GetClientArea() const {
		return GetImpl().clientArea;
	}

	void Window::SetTitle(std::string_view title) {
		GetImpl().SetTitle(title);
	}
	
	std::string Window::GetTitle() const {
		return GetImpl().GetTitle();
	}

	void Window::SetVisible(bool visible) {
		GetImpl().SetVisible(visible);
	}
	
	bool Window::IsVisible() const {
		return GetImpl().visible;
	}


	void Window::AddObserver(WindowObserver& observer) const {
		GetImpl().observers.Add(observer);
	}

	void Window::RemoveObserver(WindowObserver& observer) const {
		GetImpl().observers.Remove(observer);
	}
	

	const MouseState& Window::GetMouseState() const {
		return GetImpl().mouseState;
	}
	
	const KeysState& Window::GetKeysState() const {
		return GetImpl().keysState;
	}


	void Window::SetMouseCapture() {
		GetImpl().SetMouseCapture();
	}
	
	void Window::ReleaseMouseCapture() {
		GetImpl().ReleaseMouseCapture();
	}


	Canvas& Window::GetCanvas() {
		return *GetImpl().canvas;
	}
}