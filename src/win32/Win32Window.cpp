#include "pal/win32/Win32Window.hpp"
#include <windowsx.h>

namespace sovren::pal::win32 {
    LRESULT Win32Window::wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
        if(const auto window = fromHandle(hwnd)) {
            // translate message codes into the appropriate events
            switch(msg) {
                // window messages
                case WM_SIZE:
                    return window->performCallback(&WindowCallbacks::onWindowResize, Size2(LOWORD(lParam), HIWORD(lParam)));

                case WM_CLOSE:
                    return window->performCallback(&WindowCallbacks::onWindowClose);

                // keyboard messages
                case WM_KEYDOWN: {
                    if(!(lParam & 0x40000000)) { // do not repeat the notification if the key is held down
                        if(const auto key = mapVkeyToKeys(wParam)) {
                            return window->performCallback(&WindowCallbacks::onKeyStateChange, *key, true);
                        }
                    }
                    break;
                }

                case WM_KEYUP: {
                    if(const auto key = mapVkeyToKeys(wParam)) {
                        return window->performCallback(&WindowCallbacks::onKeyStateChange, *key, false);
                    }
                    break;
                }

                // mouse messages
                case WM_MOUSEMOVE:
                    return window->performCallback(&WindowCallbacks::onMouseMove, Point2(static_cast<float>(GET_X_LPARAM(lParam)), static_cast<float>(GET_Y_LPARAM(lParam))));

                case WM_LBUTTONDOWN:
                case WM_LBUTTONUP:
                    return window->performCallback(&WindowCallbacks::onMouseButtonStateChange, MouseButton::Left, msg == WM_LBUTTONDOWN);

                case WM_RBUTTONDOWN:
                case WM_RBUTTONUP:
                    return window->performCallback(&WindowCallbacks::onMouseButtonStateChange, MouseButton::Right, msg == WM_RBUTTONDOWN);

                case WM_MBUTTONDOWN:
                case WM_MBUTTONUP:
                    return window->performCallback(&WindowCallbacks::onMouseButtonStateChange, MouseButton::Middle, msg == WM_MBUTTONDOWN);
            }
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }


    std::optional<Key> Win32Window::mapVkeyToKeys(WPARAM vkey) noexcept {
        switch(vkey) {
            case VK_LEFT:  return Key::LeftArrow;
            case VK_RIGHT: return Key::RightArrow;
            case VK_UP:    return Key::UpArrow;
            case VK_DOWN:  return Key::DownArrow;
        }
        return std::nullopt;
    }


    Win32Window* Win32Window::fromHandle(HWND handle) {
        if(handle) {
            return reinterpret_cast<Win32Window*>(win32Check(GetWindowLongPtr(handle, GWLP_USERDATA)));
        }
        return nullptr;
    }


    Win32Window::Win32Window() {
        // initialize a new Win32 window class
        static std::weak_ptr<WndClass> defaultWndClass;
        if(defaultWndClass.expired()) {
            defaultWndClass = m_wndClass = std::make_shared<WndClass>("sovren.window", wndProc);
        } else {
            m_wndClass = defaultWndClass.lock();
        }

        // create a window handle
        m_handle.reset(win32Check(CreateWindowEx(
            0, // no extended style
            MAKEINTATOM(m_wndClass->atom()),
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

        // bind Window object to the native HWND handle
        win32Check(SetWindowLongPtr(m_handle.get(), GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this)));
    }


    void Win32Window::setTitle(std::string_view newTitle) {
        const auto tcTitle = mbToTc(newTitle);
        win32Check(SetWindowText(
            m_handle.get(),
            tcTitle.c_str()
        ));
    }


    void Win32Window::setClientSize(Size2 newSize) {
        // calculate the window size with the desired client area size
        const auto style = win32Check(GetWindowLongPtr(m_handle.get(), GWL_STYLE));
        const auto styleEx = win32Check(GetWindowLongPtr(m_handle.get(), GWL_EXSTYLE));
        const auto hasMenu = win32Check(GetMenu(m_handle.get())) != nullptr;

        RECT rc = { 0, 0, static_cast<LONG>(newSize.width), static_cast<LONG>(newSize.height) };
        win32Check(AdjustWindowRectEx(
            &rc,
            static_cast<DWORD>(style),
            hasMenu,
            static_cast<DWORD>(styleEx)
        ));

        // update the window size
        win32Check(SetWindowPos(
            m_handle.get(),
            nullptr,
            0,
            0, // no need to move the window
            rc.right - rc.left,
            rc.bottom - rc.top, // new window size
            SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER
        ));
    }


    std::string Win32Window::getTitle() const {
        const auto tcTitleLen = win32Check(GetWindowTextLength(m_handle.get())) + 1;
        if(tcTitleLen > 1) {
            tstring tcTitle(tcTitleLen, '\0');
            win32Check(GetWindowText(
                m_handle.get(),
                tcTitle.data(),
                tcTitleLen
            ));
            tcTitle.pop_back(); // remove the null character
            return tcToMb(tcTitle);
        }
        return "";
    }


    Size2 Win32Window::getClientSize() const {
        RECT r = {};
        win32Check(GetClientRect(m_handle.get(), &r));
        return { static_cast<float>(r.right), static_cast<float>(r.bottom) };
    }


    Point2 Win32Window::getMousePos() const {
        POINT p = {};
        win32Check(GetCursorPos(&p));
        win32Check(ScreenToClient(m_handle.get(), &p));
        return { static_cast<float>(p.x), static_cast<float>(p.y) };
    }
}
