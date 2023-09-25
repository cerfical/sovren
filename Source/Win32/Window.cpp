#include "Window.hpp"

#include "WinUiFactory.hpp"
#include "WinWindow.hpp"

#include <unordered_map>
#include <vector>

namespace RENI {
	class Window::Impl : public Win32::WinWindow {
	public:
		Impl() {
			canvas = Win32::WinUiFactory::Get()->MakeCanvas(*this);
		}
		~Impl() = default;

		void NotifyWhen(WindowEvent event, std::function<void()> func) {
			listeners[event].emplace_back(std::move(func));
		}
		void TriggerEvent(WindowEvent event) const {
			if(listeners.contains(event)) {
				const auto& els = listeners.equal_range(event).first->second;
				for(const auto& el : els) {
					el();
				}
			}
		}

		LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) override {
			switch(msg) {
				case WM_SHOWWINDOW: {
					OnWmShowWindow(wParam == TRUE);
					break;
				}
				case WM_SIZE: {
					OnWmSize({ LOWORD(lParam), HIWORD(lParam) });
					break;
				}
				case WM_PAINT: {
					OnWmPaint();
					break;
				}
				case WM_CLOSE: {
					OnWmClose();
					return 0;
				}
			}
			return DefWindowProc(GetHandle(), msg, wParam, lParam);
		}

		/** @brief Handle WM_SHOWWINDOW. */
		void OnWmShowWindow(bool visible) {
			if(this->visible != visible) {
				this->visible = visible;
				TriggerEvent(visible ? WindowEvent::Show : WindowEvent::Hide);
			}
		}

		/** @brief Handle WM_SIZE. */
		void OnWmSize(Extent2D clientArea) {
			if(this->clientArea != clientArea) {
				this->clientArea = clientArea;
				TriggerEvent(WindowEvent::Resize);
			}
		}
		
		/** @brief Handle WM_PAINT. */
		void OnWmPaint() {
			TriggerEvent(WindowEvent::Draw);
		}
		
		/** @brief Handle WM_CLOSE. */
		void OnWmClose() {
			TriggerEvent(WindowEvent::Close);
		}

		std::unordered_map<WindowEvent, std::vector<std::function<void()>>> listeners;
		std::unique_ptr<Canvas> canvas;

		Extent2D clientArea = { };
		bool visible = false;
	};
}

namespace RENI {
	const Window::Impl& Window::GetImpl() const {
		if(!impl) {
			impl = std::make_unique<Impl>();

			auto& implRef = *impl;
			implRef.NotifyWhen(WindowEvent::Close, [&implRef]() {
				implRef.SetVisible(false);
			});
			implRef.NotifyWhen(WindowEvent::Resize, [&implRef]() {
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

	Window::Window(std::string_view title, Extent2D size) {
		SetTitle(title);
		SetClientArea(size);
	}
	Window::~Window() = default;

	void Window::NotifyWhen(WindowEvent event, std::function<void()> func) {
		GetImpl().NotifyWhen(event, std::move(func));
	}
	void Window::TriggerEvent(WindowEvent event) {
		GetImpl().TriggerEvent(event);
	}

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

	Canvas& Window::GetCanvas() {
		return *GetImpl().canvas;
	}
}