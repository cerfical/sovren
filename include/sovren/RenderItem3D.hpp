#pragma once

#include <sovren/Mesh.hpp>
#include <sovren/Transform3D.hpp>

namespace sovren {

    struct RenderItem3D {
        const Mesh* mesh = {};
        Transform3D transform;
    };

}
