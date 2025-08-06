#pragma once

#include "../core/Point2.hpp"
#include "../core/Size2.hpp"

namespace sovren::rhi {

    struct Viewport {
        Point2f topLeft;
        Size2f size;
    };

}
