#pragma once

#include "Format.hpp"

#include <string_view>

namespace sovren::rhi {

    struct VertexAttr {
        std::string_view name;
        Format format;
    };

}
