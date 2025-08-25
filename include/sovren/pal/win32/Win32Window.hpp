#pragma once

#include <sovren/log/Log.hpp>

#include <sovren/pal/Window.hpp>
#include <sovren/pal/WindowHandler.hpp>

#include <sovren/pal/win32/WndClass.hpp>
#include <sovren/pal/win32/errors.hpp>
#include <sovren/pal/win32/strings.hpp>

#include <Windows.h>
#include <windowsx.h>
#include <winuser.h>

#include <format>
#include <memory>
#include <optional>
#include <string_view>
#include <unordered_map>

namespace sovren::pal {

    class Win32Window : public Window {
    public:
        explicit Win32Window(WindowHandler* handler, log::Log log)
            : log_(std::move(log)), handler_(handler) {
            // initialize a new Win32 window class
            static std::weak_ptr<WndClass> wndClass;
            if(wndClass.expired()) {
                wndClass = wndClass_ = std::make_shared<WndClass>("sovren.window", wndProc);
            } else {
                wndClass_ = wndClass.lock();
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
                this                 // parameter for WM_CREATE
            )));

            log_.info("Window created");
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

            log_.info("Window title changed to \"{}\"", title);
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

            log_.info("Window resized to {}", size);
        }

        [[nodiscard]]
        auto isVisible() const -> bool override {
            return static_cast<bool>(win32Check(IsWindowVisible(handle_.get())));
        }

        void setVisible(bool visible) override {
            win32Check(ShowWindow(handle_.get(), visible ? SW_SHOW : SW_HIDE));

            log_.info("Window {}", visible ? "shown" : "hidden");
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
            // bind a window to the native HWND handle at the time the window is created
            if(msg == WM_CREATE) {
                auto* const wnd = reinterpret_cast<Win32Window*>(
                    reinterpret_cast<LPCREATESTRUCT>(lParam)->lpCreateParams
                );

                wnd->log_ = wnd->log_.withTags(std::format("HWND({})", Handle(hwnd)));
                wnd->msgLog_ = wnd->log_.withTags("MSG");
                wnd->logMsg(msg, wParam, lParam);

                win32Check(SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(wnd)));
                return 0;
            }

            if(auto* const wnd = windowFromHandle(hwnd)) {
                wnd->logMsg(msg, wParam, lParam);

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
        static auto msgToText(UINT msg) -> std::string {
            static const std::unordered_map<UINT, std::string_view> names = {
                { WM_NULL, "WM_NULL" },
                { WM_CREATE, "WM_CREATE" },
                { WM_DESTROY, "WM_DESTROY" },
                { WM_MOVE, "WM_MOVE" },
                { WM_SIZE, "WM_SIZE" },
                { WM_ACTIVATE, "WM_ACTIVATE" },
                { WM_SETFOCUS, "WM_SETFOCUS" },
                { WM_KILLFOCUS, "WM_KILLFOCUS" },
                { WM_ENABLE, "WM_ENABLE" },
                { WM_SETREDRAW, "WM_SETREDRAW" },
                { WM_SETTEXT, "WM_SETTEXT" },
                { WM_GETTEXT, "WM_GETTEXT" },
                { WM_GETTEXTLENGTH, "WM_GETTEXTLENGTH" },
                { WM_PAINT, "WM_PAINT" },
                { WM_CLOSE, "WM_CLOSE" },
                { WM_QUERYENDSESSION, "WM_QUERYENDSESSION" },
                { WM_QUERYOPEN, "WM_QUERYOPEN" },
                { WM_ENDSESSION, "WM_ENDSESSION" },
                { WM_QUIT, "WM_QUIT" },
                { WM_ERASEBKGND, "WM_ERASEBKGND" },
                { WM_SYSCOLORCHANGE, "WM_SYSCOLORCHANGE" },
                { WM_SHOWWINDOW, "WM_SHOWWINDOW" },
                { WM_WININICHANGE, "WM_WININICHANGE" },
                { WM_SETTINGCHANGE, "WM_SETTINGCHANGE" },
                { WM_DEVMODECHANGE, "WM_DEVMODECHANGE" },
                { WM_ACTIVATEAPP, "WM_ACTIVATEAPP" },
                { WM_FONTCHANGE, "WM_FONTCHANGE" },
                { WM_TIMECHANGE, "WM_TIMECHANGE" },
                { WM_CANCELMODE, "WM_CANCELMODE" },
                { WM_SETCURSOR, "WM_SETCURSOR" },
                { WM_MOUSEACTIVATE, "WM_MOUSEACTIVATE" },
                { WM_CHILDACTIVATE, "WM_CHILDACTIVATE" },
                { WM_QUEUESYNC, "WM_QUEUESYNC" },
                { WM_GETMINMAXINFO, "WM_GETMINMAXINFO" },
                { WM_PAINTICON, "WM_PAINTICON" },
                { WM_ICONERASEBKGND, "WM_ICONERASEBKGND" },
                { WM_NEXTDLGCTL, "WM_NEXTDLGCTL" },
                { WM_SPOOLERSTATUS, "WM_SPOOLERSTATUS" },
                { WM_DRAWITEM, "WM_DRAWITEM" },
                { WM_MEASUREITEM, "WM_MEASUREITEM" },
                { WM_DELETEITEM, "WM_DELETEITEM" },
                { WM_VKEYTOITEM, "WM_VKEYTOITEM" },
                { WM_CHARTOITEM, "WM_CHARTOITEM" },
                { WM_SETFONT, "WM_SETFONT" },
                { WM_GETFONT, "WM_GETFONT" },
                { WM_SETHOTKEY, "WM_SETHOTKEY" },
                { WM_GETHOTKEY, "WM_GETHOTKEY" },
                { WM_QUERYDRAGICON, "WM_QUERYDRAGICON" },
                { WM_COMPAREITEM, "WM_COMPAREITEM" },
                { WM_GETOBJECT, "WM_GETOBJECT" },
                { WM_COMPACTING, "WM_COMPACTING" },
                { WM_COMMNOTIFY, "WM_COMMNOTIFY" },
                { WM_WINDOWPOSCHANGING, "WM_WINDOWPOSCHANGING" },
                { WM_WINDOWPOSCHANGED, "WM_WINDOWPOSCHANGED" },
                { WM_POWER, "WM_POWER" },
                { WM_COPYDATA, "WM_COPYDATA" },
                { WM_CANCELJOURNAL, "WM_CANCELJOURNAL" },
                { WM_NOTIFY, "WM_NOTIFY" },
                { WM_INPUTLANGCHANGEREQUEST, "WM_INPUTLANGCHANGEREQUEST" },
                { WM_INPUTLANGCHANGE, "WM_INPUTLANGCHANGE" },
                { WM_TCARD, "WM_TCARD" },
                { WM_HELP, "WM_HELP" },
                { WM_USERCHANGED, "WM_USERCHANGED" },
                { WM_NOTIFYFORMAT, "WM_NOTIFYFORMAT" },
                { WM_CONTEXTMENU, "WM_CONTEXTMENU" },
                { WM_STYLECHANGING, "WM_STYLECHANGING" },
                { WM_STYLECHANGED, "WM_STYLECHANGED" },
                { WM_DISPLAYCHANGE, "WM_DISPLAYCHANGE" },
                { WM_GETICON, "WM_GETICON" },
                { WM_SETICON, "WM_SETICON" },
                { WM_NCCREATE, "WM_NCCREATE" },
                { WM_NCDESTROY, "WM_NCDESTROY" },
                { WM_NCCALCSIZE, "WM_NCCALCSIZE" },
                { WM_NCHITTEST, "WM_NCHITTEST" },
                { WM_NCPAINT, "WM_NCPAINT" },
                { WM_NCACTIVATE, "WM_NCACTIVATE" },
                { WM_GETDLGCODE, "WM_GETDLGCODE" },
                { WM_SYNCPAINT, "WM_SYNCPAINT" },
                { WM_NCMOUSEMOVE, "WM_NCMOUSEMOVE" },
                { WM_NCLBUTTONDOWN, "WM_NCLBUTTONDOWN" },
                { WM_NCLBUTTONUP, "WM_NCLBUTTONUP" },
                { WM_NCLBUTTONDBLCLK, "WM_NCLBUTTONDBLCLK" },
                { WM_NCRBUTTONDOWN, "WM_NCRBUTTONDOWN" },
                { WM_NCRBUTTONUP, "WM_NCRBUTTONUP" },
                { WM_NCRBUTTONDBLCLK, "WM_NCRBUTTONDBLCLK" },
                { WM_NCMBUTTONDOWN, "WM_NCMBUTTONDOWN" },
                { WM_NCMBUTTONUP, "WM_NCMBUTTONUP" },
                { WM_NCMBUTTONDBLCLK, "WM_NCMBUTTONDBLCLK" },
                { WM_NCXBUTTONDOWN, "WM_NCXBUTTONDOWN" },
                { WM_NCXBUTTONUP, "WM_NCXBUTTONUP" },
                { WM_NCXBUTTONDBLCLK, "WM_NCXBUTTONDBLCLK" },
                { WM_INPUT_DEVICE_CHANGE, "WM_INPUT_DEVICE_CHANGE" },
                { WM_INPUT, "WM_INPUT" },
                { WM_KEYFIRST, "WM_KEYFIRST" },
                { WM_KEYDOWN, "WM_KEYDOWN" },
                { WM_KEYUP, "WM_KEYUP" },
                { WM_CHAR, "WM_CHAR" },
                { WM_DEADCHAR, "WM_DEADCHAR" },
                { WM_SYSKEYDOWN, "WM_SYSKEYDOWN" },
                { WM_SYSKEYUP, "WM_SYSKEYUP" },
                { WM_SYSCHAR, "WM_SYSCHAR" },
                { WM_SYSDEADCHAR, "WM_SYSDEADCHAR" },
                { WM_UNICHAR, "WM_UNICHAR" },
                { WM_KEYLAST, "WM_KEYLAST" },
                { WM_KEYLAST, "WM_KEYLAST" },
                { WM_IME_STARTCOMPOSITION, "WM_IME_STARTCOMPOSITION" },
                { WM_IME_ENDCOMPOSITION, "WM_IME_ENDCOMPOSITION" },
                { WM_IME_COMPOSITION, "WM_IME_COMPOSITION" },
                { WM_IME_KEYLAST, "WM_IME_KEYLAST" },
                { WM_INITDIALOG, "WM_INITDIALOG" },
                { WM_COMMAND, "WM_COMMAND" },
                { WM_SYSCOMMAND, "WM_SYSCOMMAND" },
                { WM_TIMER, "WM_TIMER" },
                { WM_HSCROLL, "WM_HSCROLL" },
                { WM_VSCROLL, "WM_VSCROLL" },
                { WM_INITMENU, "WM_INITMENU" },
                { WM_INITMENUPOPUP, "WM_INITMENUPOPUP" },
                { WM_GESTURE, "WM_GESTURE" },
                { WM_GESTURENOTIFY, "WM_GESTURENOTIFY" },
                { WM_MENUSELECT, "WM_MENUSELECT" },
                { WM_MENUCHAR, "WM_MENUCHAR" },
                { WM_ENTERIDLE, "WM_ENTERIDLE" },
                { WM_MENURBUTTONUP, "WM_MENURBUTTONUP" },
                { WM_MENUDRAG, "WM_MENUDRAG" },
                { WM_MENUGETOBJECT, "WM_MENUGETOBJECT" },
                { WM_UNINITMENUPOPUP, "WM_UNINITMENUPOPUP" },
                { WM_MENUCOMMAND, "WM_MENUCOMMAND" },
                { WM_CHANGEUISTATE, "WM_CHANGEUISTATE" },
                { WM_UPDATEUISTATE, "WM_UPDATEUISTATE" },
                { WM_QUERYUISTATE, "WM_QUERYUISTATE" },
                { WM_CTLCOLORMSGBOX, "WM_CTLCOLORMSGBOX" },
                { WM_CTLCOLOREDIT, "WM_CTLCOLOREDIT" },
                { WM_CTLCOLORLISTBOX, "WM_CTLCOLORLISTBOX" },
                { WM_CTLCOLORBTN, "WM_CTLCOLORBTN" },
                { WM_CTLCOLORDLG, "WM_CTLCOLORDLG" },
                { WM_CTLCOLORSCROLLBAR, "WM_CTLCOLORSCROLLBAR" },
                { WM_CTLCOLORSTATIC, "WM_CTLCOLORSTATIC" },
                { WM_MOUSEFIRST, "WM_MOUSEFIRST" },
                { WM_MOUSEMOVE, "WM_MOUSEMOVE" },
                { WM_LBUTTONDOWN, "WM_LBUTTONDOWN" },
                { WM_LBUTTONUP, "WM_LBUTTONUP" },
                { WM_LBUTTONDBLCLK, "WM_LBUTTONDBLCLK" },
                { WM_RBUTTONDOWN, "WM_RBUTTONDOWN" },
                { WM_RBUTTONUP, "WM_RBUTTONUP" },
                { WM_RBUTTONDBLCLK, "WM_RBUTTONDBLCLK" },
                { WM_MBUTTONDOWN, "WM_MBUTTONDOWN" },
                { WM_MBUTTONUP, "WM_MBUTTONUP" },
                { WM_MBUTTONDBLCLK, "WM_MBUTTONDBLCLK" },
                { WM_MOUSEWHEEL, "WM_MOUSEWHEEL" },
                { WM_XBUTTONDOWN, "WM_XBUTTONDOWN" },
                { WM_XBUTTONUP, "WM_XBUTTONUP" },
                { WM_XBUTTONDBLCLK, "WM_XBUTTONDBLCLK" },
                { WM_MOUSEHWHEEL, "WM_MOUSEHWHEEL" },
                { WM_MOUSELAST, "WM_MOUSELAST" },
                { WM_MOUSELAST, "WM_MOUSELAST" },
                { WM_MOUSELAST, "WM_MOUSELAST" },
                { WM_MOUSELAST, "WM_MOUSELAST" },
                { WM_PARENTNOTIFY, "WM_PARENTNOTIFY" },
                { WM_ENTERMENULOOP, "WM_ENTERMENULOOP" },
                { WM_EXITMENULOOP, "WM_EXITMENULOOP" },
                { WM_NEXTMENU, "WM_NEXTMENU" },
                { WM_SIZING, "WM_SIZING" },
                { WM_CAPTURECHANGED, "WM_CAPTURECHANGED" },
                { WM_MOVING, "WM_MOVING" },
                { WM_POWERBROADCAST, "WM_POWERBROADCAST" },
                { WM_DEVICECHANGE, "WM_DEVICECHANGE" },
                { WM_MDICREATE, "WM_MDICREATE" },
                { WM_MDIDESTROY, "WM_MDIDESTROY" },
                { WM_MDIACTIVATE, "WM_MDIACTIVATE" },
                { WM_MDIRESTORE, "WM_MDIRESTORE" },
                { WM_MDINEXT, "WM_MDINEXT" },
                { WM_MDIMAXIMIZE, "WM_MDIMAXIMIZE" },
                { WM_MDITILE, "WM_MDITILE" },
                { WM_MDICASCADE, "WM_MDICASCADE" },
                { WM_MDIICONARRANGE, "WM_MDIICONARRANGE" },
                { WM_MDIGETACTIVE, "WM_MDIGETACTIVE" },
                { WM_MDISETMENU, "WM_MDISETMENU" },
                { WM_ENTERSIZEMOVE, "WM_ENTERSIZEMOVE" },
                { WM_EXITSIZEMOVE, "WM_EXITSIZEMOVE" },
                { WM_DROPFILES, "WM_DROPFILES" },
                { WM_MDIREFRESHMENU, "WM_MDIREFRESHMENU" },
                { WM_POINTERDEVICECHANGE, "WM_POINTERDEVICECHANGE" },
                { WM_POINTERDEVICEINRANGE, "WM_POINTERDEVICEINRANGE" },
                { WM_POINTERDEVICEOUTOFRANGE, "WM_POINTERDEVICEOUTOFRANGE" },
                { WM_TOUCH, "WM_TOUCH" },
                { WM_NCPOINTERUPDATE, "WM_NCPOINTERUPDATE" },
                { WM_NCPOINTERDOWN, "WM_NCPOINTERDOWN" },
                { WM_NCPOINTERUP, "WM_NCPOINTERUP" },
                { WM_POINTERUPDATE, "WM_POINTERUPDATE" },
                { WM_POINTERDOWN, "WM_POINTERDOWN" },
                { WM_POINTERUP, "WM_POINTERUP" },
                { WM_POINTERENTER, "WM_POINTERENTER" },
                { WM_POINTERLEAVE, "WM_POINTERLEAVE" },
                { WM_POINTERACTIVATE, "WM_POINTERACTIVATE" },
                { WM_POINTERCAPTURECHANGED, "WM_POINTERCAPTURECHANGED" },
                { WM_TOUCHHITTESTING, "WM_TOUCHHITTESTING" },
                { WM_POINTERWHEEL, "WM_POINTERWHEEL" },
                { WM_POINTERHWHEEL, "WM_POINTERHWHEEL" },
                { WM_IME_SETCONTEXT, "WM_IME_SETCONTEXT" },
                { WM_IME_NOTIFY, "WM_IME_NOTIFY" },
                { WM_IME_CONTROL, "WM_IME_CONTROL" },
                { WM_IME_COMPOSITIONFULL, "WM_IME_COMPOSITIONFULL" },
                { WM_IME_SELECT, "WM_IME_SELECT" },
                { WM_IME_CHAR, "WM_IME_CHAR" },
                { WM_IME_REQUEST, "WM_IME_REQUEST" },
                { WM_IME_KEYDOWN, "WM_IME_KEYDOWN" },
                { WM_IME_KEYUP, "WM_IME_KEYUP" },
                { WM_MOUSEHOVER, "WM_MOUSEHOVER" },
                { WM_MOUSELEAVE, "WM_MOUSELEAVE" },
                { WM_NCMOUSEHOVER, "WM_NCMOUSEHOVER" },
                { WM_NCMOUSELEAVE, "WM_NCMOUSELEAVE" },
                { WM_WTSSESSION_CHANGE, "WM_WTSSESSION_CHANGE" },
                { WM_TABLET_FIRST, "WM_TABLET_FIRST" },
                { WM_TABLET_LAST, "WM_TABLET_LAST" },
                { WM_CUT, "WM_CUT" },
                { WM_COPY, "WM_COPY" },
                { WM_PASTE, "WM_PASTE" },
                { WM_CLEAR, "WM_CLEAR" },
                { WM_UNDO, "WM_UNDO" },
                { WM_RENDERFORMAT, "WM_RENDERFORMAT" },
                { WM_RENDERALLFORMATS, "WM_RENDERALLFORMATS" },
                { WM_DESTROYCLIPBOARD, "WM_DESTROYCLIPBOARD" },
                { WM_DRAWCLIPBOARD, "WM_DRAWCLIPBOARD" },
                { WM_PAINTCLIPBOARD, "WM_PAINTCLIPBOARD" },
                { WM_VSCROLLCLIPBOARD, "WM_VSCROLLCLIPBOARD" },
                { WM_SIZECLIPBOARD, "WM_SIZECLIPBOARD" },
                { WM_ASKCBFORMATNAME, "WM_ASKCBFORMATNAME" },
                { WM_CHANGECBCHAIN, "WM_CHANGECBCHAIN" },
                { WM_HSCROLLCLIPBOARD, "WM_HSCROLLCLIPBOARD" },
                { WM_QUERYNEWPALETTE, "WM_QUERYNEWPALETTE" },
                { WM_PALETTEISCHANGING, "WM_PALETTEISCHANGING" },
                { WM_PALETTECHANGED, "WM_PALETTECHANGED" },
                { WM_HOTKEY, "WM_HOTKEY" },
                { WM_PRINT, "WM_PRINT" },
                { WM_PRINTCLIENT, "WM_PRINTCLIENT" },
                { WM_APPCOMMAND, "WM_APPCOMMAND" },
                { WM_THEMECHANGED, "WM_THEMECHANGED" },
                { WM_CLIPBOARDUPDATE, "WM_CLIPBOARDUPDATE" },
                { WM_DWMCOMPOSITIONCHANGED, "WM_DWMCOMPOSITIONCHANGED" },
                { WM_DWMNCRENDERINGCHANGED, "WM_DWMNCRENDERINGCHANGED" },
                { WM_DWMCOLORIZATIONCOLORCHANGED, "WM_DWMCOLORIZATIONCOLORCHANGED" },
                { WM_DWMWINDOWMAXIMIZEDCHANGE, "WM_DWMWINDOWMAXIMIZEDCHANGE" },
                { WM_DWMSENDICONICTHUMBNAIL, "WM_DWMSENDICONICTHUMBNAIL" },
                { WM_DWMSENDICONICLIVEPREVIEWBITMAP, "WM_DWMSENDICONICLIVEPREVIEWBITMAP" },
                { WM_GETTITLEBARINFOEX, "WM_GETTITLEBARINFOEX" },
                { WM_HANDHELDFIRST, "WM_HANDHELDFIRST" },
                { WM_HANDHELDLAST, "WM_HANDHELDLAST" },
                { WM_AFXFIRST, "WM_AFXFIRST" },
                { WM_AFXLAST, "WM_AFXLAST" },
                { WM_PENWINFIRST, "WM_PENWINFIRST" },
                { WM_PENWINLAST, "WM_PENWINLAST" },
            };

            if(const auto it = names.find(msg); it != names.end()) {
                return std::format("{}", it->second);
            }

            if(msg >= WM_USER && msg < WM_APP) {
                return std::format("WM_USER(+{})", msg - WM_USER);
            }
            if(msg >= WM_APP && msg < 0xC000) {
                return std::format("WM_APP(+{})", msg - WM_APP);
            }
            return std::format("WM_NULL(+{})", msg);
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

        void logMsg(UINT msg, WPARAM wParam, LPARAM lParam) {
            const auto text = msgToText(msg);
            msgLog_.info("{}: {:#x}, {:#x}", text, wParam, lParam);
        }

        class HwndDeleter {
        public:
            using pointer = HWND;

            void operator()(pointer handle) {
                auto* const wnd = windowFromHandle(handle);
                win32Check(DestroyWindow(handle));
                if(wnd) {
                    wnd->log_.info("Window destroyed");
                }
            }
        };

        log::Log log_;
        log::Log msgLog_;

        std::shared_ptr<WndClass> wndClass_;
        std::unique_ptr<HWND, HwndDeleter> handle_;

        WindowHandler* handler_ = {};
    };

}
