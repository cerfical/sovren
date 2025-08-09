#include "pal/win32/Win32EventPump.hpp"
#include "pal/win32/Win32Window.hpp"
#include "rhi/dx11/Dx11Device.hpp"

#include "pal/EventPump.hpp"
#include "pal/Window.hpp"
#include "pal/WindowHandler.hpp"
#include "rhi/Device.hpp"

#include <memory>

namespace sovren {
    auto Window::create(WindowHandler* handler) -> std::unique_ptr<Window> {
        return std::make_unique<win32::Win32Window>(handler);
    }

    auto EventPump::create() -> std::unique_ptr<EventPump> {
        return std::make_unique<win32::Win32EventPump>();
    }

    auto rhi::Device::create() -> std::unique_ptr<rhi::Device> {
        return std::make_unique<rhi::Dx11Device>();
    }
}
