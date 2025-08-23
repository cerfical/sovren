#pragma once

#include <sovren/pal/MouseButton.hpp>

#include <sovren/util/Point2.hpp>

#include <sovren/MouseEventType.hpp>

namespace sovren {

    struct MouseEvent {
        Point2i pos;
        pal::MouseButton button = {};
        MouseEventType type = {};
    };

}
