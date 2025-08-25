#pragma once

#include <sovren/util/Color.hpp>

#include <sovren/Shape.hpp>
#include <sovren/Transform2D.hpp>

namespace sovren {

    struct RenderItem2D {
        Shape shape;
        Color outlineColor;
        Color fillColor;
        Transform2D transform;
    };

}
