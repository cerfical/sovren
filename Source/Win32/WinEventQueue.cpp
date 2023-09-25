#include "WinEventQueue.hpp"

#include "WinWindowSystem.hpp"
#include "WinUtils.hpp"
#include "Window.hpp"

#include <windowsx.h>

#define RENI_VKEY_MAPPING_LIST \
	RENI_VKEY_MAPPING(VK_LEFT, LeftArrow) \
	RENI_VKEY_MAPPING(VK_RIGHT, RightArrow) \
	RENI_VKEY_MAPPING(VK_UP, UpArrow) \
	RENI_VKEY_MAPPING(VK_DOWN, DownArrow)

namespace RENI {
	NilEvent WinEventQueue::nilEvent;


	std::optional<Keys> WinEventQueue::mapVKeyToKey(WPARAM vkey) noexcept {

#define RENI_VKEY_MAPPING(vkey, key) case vkey: { return Keys::key; }
		switch(vkey) {
			RENI_VKEY_MAPPING_LIST
		}
#undef RENI_VKEY_MAPPING

		return { };
	}

	LRESULT WinEventQueue::wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
		// translate message codes into the appropriate events
		if(const auto queue = instance(); queue->insideMsgHandler()) {
			if(const auto window = WinWindowSystem::instance()->windowFromHandle(hwnd)) {
				switch(msg) {
				// window messages
					case WM_SIZE: {
						const auto& oldSize = window->clientSize();
						const auto newSize = Size2D(LOWORD(lParam), HIWORD(lParam));
						if(newSize != oldSize) {
							queue->yieldEvent(ResizeEvent(newSize, oldSize, window));
						}
						return 0;
					}

					case WM_CLOSE: {
						queue->yieldEvent(CloseEvent(window));
						return 0;
					}

				// keyboard messages
					case WM_KEYDOWN: {
						if(!(lParam & 0x40000000)) { // do not repeat the notification if the key is held down
							if(const auto key = mapVKeyToKey(wParam); key) {
								queue->yieldEvent(KeyPressEvent(*key, window));
							}
						}
						return 0;
					}

					case WM_KEYUP: {
						if(const auto key = mapVKeyToKey(wParam); key) {
							queue->yieldEvent(KeyReleaseEvent(*key, window));
						}
						return 0;
					}

				// mouse messages
					case WM_LBUTTONDOWN: {
						queue->yieldEvent(MousePressEvent(MouseButtons::Left, window));
						return 0;
					}

					case WM_LBUTTONUP: {
						queue->yieldEvent(MouseReleaseEvent(MouseButtons::Left, window));
						return 0;
					}

					case WM_RBUTTONDOWN: {
						queue->yieldEvent(MousePressEvent(MouseButtons::Right, window));
						return 0;
					}

					case WM_RBUTTONUP: {
						queue->yieldEvent(MouseReleaseEvent(MouseButtons::Right, window));
						return 0;
					}

					case WM_MBUTTONDOWN: {
						queue->yieldEvent(MousePressEvent(MouseButtons::Middle, window));
						return 0;
					}

					case WM_MBUTTONUP: {
						queue->yieldEvent(MouseReleaseEvent(MouseButtons::Middle, window));
						return 0;
					}

					case WM_MOUSEMOVE: {
						const auto& oldPos = window->mouse().cursorPos();
						const auto newPos = Point2D(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

						if(newPos != oldPos) {
							queue->yieldEvent(MouseMoveEvent(newPos, oldPos, window));
						}
						return 0;
					}
				}
			}
		}

		// watch for visibility changes to exit the message loop if none of the windows are visible
		if(msg == WM_SHOWWINDOW) {
			static int visibleWindows = 0;
			if(wParam) {
				visibleWindows++;
			} else {
				visibleWindows--;
				if(visibleWindows == 0) {
					safeWin32ApiCall(PostQuitMessage, 0);
				}
			}
			return 0;
		}

		return DefWindowProc(hwnd, msg, wParam, lParam);
	}


	void WinEventQueue::msgHandler(LPVOID) noexcept {
		const auto queue = instance();
		while(true) {
			if(MSG msg = { }; PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
				if(msg.message != WM_QUIT) {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				} else {
					queue->yieldEvent(QuitEvent(static_cast<int>(msg.wParam)));
				}
			} else {
				queue->yieldNoEvents();
			}
		}
	}

	bool WinEventQueue::insideMsgHandler() const {
		return safeWin32ApiCall(GetCurrentFiber) == m_msgHandlerFiber.get();
	}


	WinEventQueue* WinEventQueue::instance() {
		static WinEventQueue queue;
		return &queue;
	}

	WinEventQueue::WinEventQueue() {
		// convert the current thread to a fiber if it's not
		if(!safeWin32ApiCall(IsThreadAFiber)) {
			safeWin32ApiCall(ConvertThreadToFiber, nullptr);
		}
		m_mainFiber.reset(safeWin32ApiCall(GetCurrentFiber));

		// create a new fiber for message handling
		m_msgHandlerFiber.reset(safeWin32ApiCall(CreateFiber, 0, msgHandler, nullptr));
	}


	void WinEventQueue::yieldEvent(const Event& event) {
		m_event = const_cast<Event*>(&event);
		safeWin32ApiCall(SwitchToFiber, m_mainFiber.get());
	}

	void WinEventQueue::yieldNoEvents() {
		safeWin32ApiCall(SwitchToFiber, m_mainFiber.get());
	}


	void WinEventQueue::ThreadFiberDeleter::operator()(pointer fiber) const {
		safeWin32ApiCall(ConvertFiberToThread);
	}

	void WinEventQueue::FiberDeleter::operator()(pointer fiber) const {
		safeWin32ApiCall(DeleteFiber, fiber);
	}


	void WinEventQueue::postQuitEvent(int exitCode) {
		safeWin32ApiCall(PostQuitMessage, exitCode);
	}

	void WinEventQueue::waitForEvents() const {
		while(!*peekEvent()) {
			safeWin32ApiCall(WaitMessage);
		}
	}


	Event* WinEventQueue::getEvent() {
		if(peekEvent()->type() != Events::Quit) {
			return std::exchange(m_event, &nilEvent);
		}
		return m_event;
	}

	Event* WinEventQueue::peekEvent() const {
		if(!*m_event) {
			safeWin32ApiCall(SwitchToFiber, m_msgHandlerFiber.get());
		}
		return m_event;
	}
}