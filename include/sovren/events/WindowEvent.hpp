#pragma once

#include <sovren/util/Size2.hpp>

#include <sovren/events/WindowEventType.hpp>

namespace sovren {

    struct WindowEvent {
        Size2i size;
        WindowEventType type = {};
    };

}
