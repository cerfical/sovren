#pragma once

#include "ShaderLang.hpp"

#include <cstddef>
#include <span>

namespace sovren::rhi {

    struct ShaderCode {
        std::span<const std::byte> data;
        ShaderLang lang = {};
    };

}
