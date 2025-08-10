#pragma once

#include <sovren/util/Point2.hpp>
#include <sovren/util/Size2.hpp>

namespace sovren::rhi {

    struct Viewport {
        Point2f topLeft;
        Size2f size;
    };

}
