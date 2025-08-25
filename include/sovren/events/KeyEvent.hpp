#pragma once

#include <sovren/pal/Key.hpp>

#include <sovren/events/KeyEventType.hpp>

namespace sovren {

    struct KeyEvent {
        pal::Key key = {};
        KeyEventType type = {};
    };

}
