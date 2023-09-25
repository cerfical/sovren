#include "Window.hpp"

#include "WinUiFactory.hpp"
#include "WinWindow.hpp"

#include <algorithm>
#include <unordered_map>
#include <algorithm>
#include <ranges>
#include <vector>

#include <Windowsx.h>

namespace {
	using namespace RENI;

	/**
	 * @brief Adapter for EventObserver to use callback functions as event handlers.
	 */
	class EventCallbacks : public EventObserver {
	public:
		/** @{ */
		/** @brief Construct a new EventCallbacks. */
		EventCallbacks() = default;

		/** @brief Destroy the EventCallbacks. */
		~EventCallbacks() = default;
		/** @} */

		/** @{ */
		EventCallbacks(const EventCallbacks&) = delete;
		EventCallbacks& operator=(const EventCallbacks&) = delete;
		/** @} */

		/** @{ */
		EventCallbacks(EventCallbacks&&) = delete;
		EventCallbacks& operator=(EventCallbacks&&) = delete;
		/** @} */

		/** @{ */
		/** @brief Register a new callback to be called when the event of interest occurs. */
		void RegisterCallback(EventTypes event, std::function<void()> func) {
			callbacks[event].emplace_back(std::move(func));
		}
		/** @} */

	private:
		/** @{ */
		/** @brief Call all callbacks for an event. */
		void TriggerEvent(EventTypes event) {
			if(callbacks.contains(event)) {
				const auto& eventCallbacks = callbacks.equal_range(event).first->second;
				for(const auto& eventCallback : eventCallbacks) {
					eventCallback();
				}
			}
		}
		/** @} */

		/** @{ */
		void OnMouseMove() override {
			TriggerEvent(EventTypes::MouseMove);
		}

		void OnWindowShow() override {
			TriggerEvent(EventTypes::WindowShow);
		}
		void OnWindowHide() override {
			TriggerEvent(EventTypes::WindowHide);
		}

		void OnWindowClose() override {
			TriggerEvent(EventTypes::WindowClose);
		}
		void OnWindowResize() override {
			TriggerEvent(EventTypes::WindowResize);
		}
		void OnWindowDraw() override {
			TriggerEvent(EventTypes::WindowDraw);
		}
		/** @} */

		std::unordered_map<
			EventTypes, std::vector<std::function<void()>>
		> callbacks; /**< @brief Maps every possible event type to its corresponding list of callback functions. */
	};
}

namespace RENI {
	class Window::Impl : public Win32::WinWindow {
	public:
		/** @{ */
		/** @brief Construct a new Impl object. */
		Impl() {
			canvas = Win32::WinUiFactory::Get()->CreateCanvas(*this);
			observers.push_back(&callbacks);
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

		std::vector<EventObserver*> observers;
		std::unique_ptr<Canvas> canvas;
		EventCallbacks callbacks;

		MouseInput mouseState;

		Extent2D clientArea = { };
		bool visible = false;

	private:
		/** @{ */
		/** @brief Call the specified event handler on all attached observers. */
		void TriggerEvent(void (EventObserver::*event)()) {
			for(auto observer : observers) {
				(observer->*event)();
			}
		}
		/** @} */

		/** @{ */
		LRESULT HandleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) override {
			switch(msg) {
				case WM_LBUTTONDOWN: { OnWmLButtonDown(); break; }
				case WM_LBUTTONUP: { OnWmLButtonUp(); break; }
				case WM_MBUTTONDOWN: { OnWmMButtonDown(); break; }
				case WM_MBUTTONUP: { OnWmMButtonUp(); break; }
				case WM_RBUTTONDOWN: { OnWmRButtonDown(); break; }
				case WM_RBUTTONUP: { OnWmRButtonUp(); break; }
				case WM_MOUSEMOVE: { OnWmMouseMove({ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) }); break; }
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
		void OnWmMouseMove(Point2D cursorPos) {
			if(mouseState.GetCursorPos() != cursorPos) {
				mouseState.SetCursorPos(cursorPos);
				TriggerEvent(&EventObserver::OnMouseMove);
			}
		}

		/** @brief WM_SIZE message. */
		void OnWmSize(Extent2D clientArea) {
			if(this->clientArea != clientArea) {
				this->clientArea = clientArea;
				TriggerEvent(&EventObserver::OnWindowResize);
			}
		}
		/** @brief WM_PAINT message. */
		void OnWmPaint() {
			TriggerEvent(&EventObserver::OnWindowDraw);
			ValidateRect(GetHandle(), NULL);
		}

		/** @brief WM_SHOWWINDOW message. */
		void OnWmShowWindow(bool visible) {
			if(this->visible != visible) {
				this->visible = visible;
				TriggerEvent(visible ? &EventObserver::OnWindowShow : &EventObserver::OnWindowHide);
			}
		}
		/** @brief WM_CLOSE message. */
		void OnWmClose() {
			TriggerEvent(&EventObserver::OnWindowClose);
		}
		/** @} */
	};
}

namespace RENI {
	const Window::Impl& Window::GetImpl() const {
		if(!impl) {
			impl = std::make_unique<Impl>();

			auto& implRef = *impl;
			implRef.callbacks.RegisterCallback(EventTypes::WindowClose, [&implRef]() {
				implRef.SetVisible(false);
			});
			implRef.callbacks.RegisterCallback(EventTypes::WindowResize, [&implRef]() {
				implRef.canvas->Resize(implRef.GetClientArea());
			});
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

	void Window::RegisterObserver(EventObserver& observer) {
		const auto reversed = std::ranges::reverse_view(GetImpl().observers);
		if(std::ranges::find(reversed, &observer) == reversed.end()) {
			GetImpl().observers.push_back(&observer);
		}
	}
	void Window::UnregisterObserver(EventObserver& observer) {
		const auto reversed = std::ranges::reverse_view(GetImpl().observers);
		if(const auto it = std::ranges::find(reversed, &observer); it != reversed.end()) {
			GetImpl().observers.erase(std::next(it).base());
		}
	}
	
	void Window::NotifyWhen(EventTypes event, std::function<void()> func) {
		GetImpl().callbacks.RegisterCallback(event, std::move(func));
	}

	const MouseInput& Window::GetMouseState() const {
		return GetImpl().mouseState;
	}
	const KeyInput& Window::GetKeysState() const {
		return { };
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