#pragma once

#include "../util/Point2.hpp"
#include "../util/Size2.hpp"

namespace sovren::rhi {

    struct Viewport {
        Point2f topLeft;
        Size2f size;
    };

}
