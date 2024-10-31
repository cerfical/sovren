#pragma once

#include "../Window.hpp"

#include "WndClass.hpp"
#include "error_util.hpp"
#include "string_util.hpp"

#include <Windows.h>
#include <windowsx.h>
#include <winuser.h>

#include <optional>

namespace sovren::win32 {

    class Win32Window : public Window {
    public:

        Win32Window() {
            // initialize a new Win32 window class
            static std::weak_ptr<WndClass> defaultWndClass;
            if(defaultWndClass.expired()) {
                defaultWndClass = wndClass_ = std::make_shared<WndClass>("sovren.window", wndProc);
            } else {
                wndClass_ = defaultWndClass.lock();
            }

            // create a window handle
            handle_.reset(win32Check(CreateWindowEx(
                0, // no extended style
                MAKEINTATOM(wndClass_->atom()),
                nullptr,             // no title
                WS_OVERLAPPEDWINDOW, // simple overlapped window
                CW_USEDEFAULT,       // default x-position
                CW_USEDEFAULT,       // default y-position
                CW_USEDEFAULT,       // default width
                CW_USEDEFAULT,       // default height
                nullptr,             // no parent
                nullptr,             // no menu
                nullptr,             // current module
                nullptr              // no parameter for WM_CREATE
            )));
        }


        void setEventHandler(EventHandler* eventHandler) override {
            // bind EventHandler object to the native HWND handle
            win32Check(SetWindowLongPtr(handle_.get(), GWLP_USERDATA, reinterpret_cast<LONG_PTR>(eventHandler)));
        }


        [[nodiscard]]
        auto title() const -> std::string override {
            const auto tcTitleLen = win32Check(GetWindowTextLength(handle_.get())) + 1;
            if(tcTitleLen > 1) {
                TString tcTitle(tcTitleLen, '\0');
                win32Check(GetWindowText(
                    handle_.get(),
                    tcTitle.data(),
                    tcTitleLen
                ));
                tcTitle.pop_back(); // remove the null character
                return tcToMb(tcTitle);
            }
            return "";
        }


        void setTitle(std::string_view title) override {
            const auto tcTitle = mbToTc(title);
            win32Check(SetWindowText(
                handle_.get(),
                tcTitle.c_str()
            ));
        }


        [[nodiscard]]
        auto size() const -> Size2 override {
            RECT r = {};
            win32Check(GetClientRect(handle_.get(), &r));
            return { .width = r.right, .height = r.bottom };
        }


        void setSize(Size2 size) override {
            // calculate the window size with the desired client area size
            const auto style = win32Check(GetWindowLongPtr(handle_.get(), GWL_STYLE));
            const auto styleEx = win32Check(GetWindowLongPtr(handle_.get(), GWL_EXSTYLE));
            const auto hasMenu = win32Check(GetMenu(handle_.get())) != nullptr;

            RECT rc = { 0, 0, static_cast<LONG>(size.width), static_cast<LONG>(size.height) };
            win32Check(AdjustWindowRectEx(
                &rc,
                static_cast<DWORD>(style),
                static_cast<BOOL>(hasMenu),
                static_cast<DWORD>(styleEx)
            ));

            // update the window size
            win32Check(SetWindowPos(
                handle_.get(),
                nullptr,
                0,
                0, // no need to move the window
                rc.right - rc.left,
                rc.bottom - rc.top, // new window size
                SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER
            ));
        }


        [[nodiscard]]
        auto isVisible() const -> bool override {
            return static_cast<bool>(win32Check(IsWindowVisible(handle_.get())));
        }


        void setVisible(bool visible) override {
            win32Check(ShowWindow(handle_.get(), visible ? SW_SHOW : SW_HIDE));
        }


        [[nodiscard]]
        auto mousePos() const -> Point2 override {
            POINT p = {};
            win32Check(GetCursorPos(&p));
            win32Check(ScreenToClient(handle_.get(), &p));
            return { .x = p.x, .y = p.y };
        }


        [[nodiscard]]
        auto handle() const -> WindowHandle override {
            return handle_.get();
        }


    private:
        static auto wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept -> LRESULT {
            if(auto* const handler = handlerFromHandle(hwnd)) {
                // translate message codes into the appropriate events
                switch(msg) {
                    // window messages
                    case WM_SIZE:  return event(handler, &EventHandler::onWindowResize);
                    case WM_CLOSE: return event(handler, &EventHandler::onWindowClose);

                    // keyboard messages
                    case WM_KEYDOWN: {
                        if((lParam & 0x40000000) == 0) { // do not repeat the notification if the key is held down
                            if(const auto key = mapVkeyToKeys(wParam)) {
                                return event(handler, &EventHandler::onKeyDown, *key);
                            }
                        }
                        break;
                    }

                    case WM_KEYUP: {
                        if(const auto key = mapVkeyToKeys(wParam)) {
                            return event(handler, &EventHandler::onKeyUp, *key);
                        }
                        break;
                    }

                    // mouse messages
                    case WM_MOUSEMOVE: return event(handler, &EventHandler::onMouseMove);

                    case WM_LBUTTONDOWN:
                    case WM_LBUTTONUP:
                        return event(
                            handler,
                            msg == WM_LBUTTONDOWN ? &EventHandler::onMouseButtonDown : &EventHandler::onMouseButtonUp,
                            MouseButtons::Left
                        );

                    case WM_RBUTTONDOWN:
                    case WM_RBUTTONUP:
                        return event(
                            handler,
                            msg == WM_RBUTTONDOWN ? &EventHandler::onMouseButtonDown : &EventHandler::onMouseButtonUp,
                            MouseButtons::Right
                        );

                    case WM_MBUTTONDOWN:
                    case WM_MBUTTONUP:
                        return event(
                            handler,
                            msg == WM_MBUTTONDOWN ? &EventHandler::onMouseButtonDown : &EventHandler::onMouseButtonUp,
                            MouseButtons::Middle
                        );

                    default: break;
                }
            }
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }


        [[nodiscard]]
        static auto mapVkeyToKeys(WPARAM vkey) noexcept -> std::optional<Keys> {
            switch(vkey) {
                case VK_LEFT:  return Keys::LeftArrow;
                case VK_RIGHT: return Keys::RightArrow;
                case VK_UP:    return Keys::UpArrow;
                case VK_DOWN:  return Keys::DownArrow;
                default:       return std::nullopt;
            }
        }


        [[nodiscard]]
        static auto handlerFromHandle(HWND hwnd) -> EventHandler* {
            if(hwnd) {
                SetLastError(0);
                if(const auto h = GetWindowLongPtr(hwnd, GWLP_USERDATA)) {
                    return reinterpret_cast<EventHandler*>(h);
                }
                if(const auto err = GetLastError()) {
                    raiseError(err);
                }
            }
            return nullptr;
        }


        template <typename... Args, typename... Types>
        static auto event(EventHandler* handler, void (EventHandler::*event)(Types...), Args&&... args) -> LRESULT {
            if(handler) {
                (handler->*event)(std::forward<Args>(args)...);
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

        std::shared_ptr<WndClass> wndClass_;
        std::unique_ptr<HWND, HwndDeleter> handle_;
    };

}
