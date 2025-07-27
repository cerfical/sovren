#include "pal/win32/Win32EventPump.hpp"
#include "pal/win32/Win32Window.hpp"

#include "rhi/dx11/Dx11Device.hpp"

#include <memory>

namespace sovren {
    auto Window::create() -> std::unique_ptr<Window> {
        return std::make_unique<win32::Win32Window>();
    }

    auto EventPump::create() -> std::unique_ptr<EventPump> {
        return std::make_unique<win32::Win32EventPump>();
    }

    auto rhi::Device::create() -> std::unique_ptr<rhi::Device> {
        return std::make_unique<rhi::Dx11Device>();
    }
}
