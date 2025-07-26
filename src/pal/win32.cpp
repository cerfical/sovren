#include "pal/win32/Win32EventPump.hpp"
#include "pal/win32/Win32Window.hpp"

#include <memory>

namespace sovren {
    auto Window::create() -> std::unique_ptr<Window> {
        return std::make_unique<win32::Win32Window>();
    }

    auto EventPump::create() -> std::unique_ptr<EventPump> {
        return std::make_unique<win32::Win32EventPump>();
    }
}
