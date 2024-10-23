#include "pal/win32/Win32Platform.hpp"

#include "pal/win32/Win32EventPoller.hpp"
#include "pal/win32/Win32Window.hpp"

#include "rhi/dx/DxRenderBackend.hpp"

namespace sovren::pal::win32 {
    std::unique_ptr<Window> Win32Platform::createWindow() {
        return std::make_unique<Win32Window>();
    }

    std::unique_ptr<EventPoller> Win32Platform::createEventPoller() {
        return std::make_unique<Win32EventPoller>();
    }

    std::unique_ptr<rhi::RenderBackend> Win32Platform::createRenderBackend() {
        return std::make_unique<rhi::dx::DxRenderBackend>();
    }
}
