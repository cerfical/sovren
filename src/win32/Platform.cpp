#include "pal/win32/Win32Platform.hpp"

namespace sovren::pal {
    Platform* Platform::get() {
        static win32::Win32Platform platform;
        return &platform;
    }
}
