#pragma once

#include <sovren/pal/Window.hpp>
#include <sovren/pal/WindowHandler.hpp>

#include <sovren/pal/win32/WndClass.hpp>
#include <sovren/pal/win32/errors.hpp>
#include <sovren/pal/win32/strings.hpp>

#include <Windows.h>
#include <windowsx.h>
#include <winuser.h>

#include <memory>
#include <optional>

namespace sovren::pal {

    class Win32Window : public Window {
    public:
        explicit Win32Window(WindowHandler* handler)
            : handler_(handler) {
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

            // bind Window to the native HWND handle
            win32Check(SetWindowLongPtr(handle_.get(), GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this)));
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
        auto size() const -> Size2i override {
            RECT r = {};
            win32Check(GetClientRect(handle_.get(), &r));
            return { r.right, r.bottom };
        }

        void setSize(Size2i size) override {
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
        auto mousePos() const -> Point2i override {
            POINT p = {};
            win32Check(GetCursorPos(&p));
            win32Check(ScreenToClient(handle_.get(), &p));
            return { p.x, p.y };
        }

        [[nodiscard]]
        auto handle() const -> Handle override {
            return handle_.get();
        }

    private:
        [[nodiscard]]
        static auto wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept -> LRESULT {
            if(auto* const wnd = windowFromHandle(hwnd)) {
                // translate message codes into the appropriate events
                switch(msg) {
                    // window messages
                    case WM_SIZE:  return wnd->event(&WindowHandler::windowResize);
                    case WM_CLOSE: return wnd->event(&WindowHandler::windowClose);

                    // keyboard messages
                    case WM_KEYDOWN: {
                        if((lParam & 0x40000000) == 0) { // do not repeat the notification if the key is held down
                            if(const auto key = mapVkeyToKey(wParam)) {
                                return wnd->event(&WindowHandler::keyDown, *key);
                            }
                        }
                        break;
                    }

                    case WM_KEYUP: {
                        if(const auto key = mapVkeyToKey(wParam)) {
                            return wnd->event(&WindowHandler::keyUp, *key);
                        }
                        break;
                    }

                    // mouse messages
                    case WM_MOUSEMOVE: return wnd->event(&WindowHandler::mouseMove);

                    case WM_LBUTTONDOWN:
                    case WM_LBUTTONUP:
                        return wnd->event(
                            msg == WM_LBUTTONDOWN
                                ? &WindowHandler::mouseButtonDown
                                : &WindowHandler::mouseButtonUp,
                            MouseButton::Left
                        );

                    case WM_RBUTTONDOWN:
                    case WM_RBUTTONUP:
                        return wnd->event(
                            msg == WM_RBUTTONDOWN
                                ? &WindowHandler::mouseButtonDown
                                : &WindowHandler::mouseButtonUp,
                            MouseButton::Right
                        );

                    case WM_MBUTTONDOWN:
                    case WM_MBUTTONUP:
                        return wnd->event(
                            msg == WM_MBUTTONDOWN
                                ? &WindowHandler::mouseButtonDown
                                : &WindowHandler::mouseButtonUp,
                            MouseButton::Middle
                        );

                    default: break;
                }
            }
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }

        [[nodiscard]]
        static auto mapVkeyToKey(WPARAM vkey) noexcept -> std::optional<Key> {
            switch(vkey) {
                case VK_LEFT:  return Key::LeftArrow;
                case VK_RIGHT: return Key::RightArrow;
                case VK_UP:    return Key::UpArrow;
                case VK_DOWN:  return Key::DownArrow;
                default:       return std::nullopt;
            }
        }

        [[nodiscard]]
        static auto windowFromHandle(HWND hwnd) -> Win32Window* {
            if(hwnd) {
                SetLastError(0);
                if(const auto wnd = GetWindowLongPtr(hwnd, GWLP_USERDATA)) {
                    return reinterpret_cast<Win32Window*>(wnd);
                }
                if(const auto err = GetLastError()) {
                    raiseError(err);
                }
            }
            return nullptr;
        }

        template <typename... Args, typename... Types>
        [[nodiscard]]
        auto event(void (WindowHandler::*event)(Types...), Args&&... args) -> LRESULT {
            if(handler_) {
                (handler_->*event)(std::forward<Args>(args)...);
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

        WindowHandler* handler_ = {};
    };

}
