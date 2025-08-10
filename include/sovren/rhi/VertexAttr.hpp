#pragma once

#include <sovren/rhi/Format.hpp>

#include <string_view>

namespace sovren::rhi {

    struct VertexAttr {
        std::string_view name;
        Format format;
    };

}
