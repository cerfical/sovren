#pragma once

#include "AccessModes.hpp"
#include "BindModes.hpp"

#include <cstddef>
#include <span>

namespace sovren::rhi {

    struct BufferDesc {
        std::span<const std::byte> data;
        std::size_t size = {};

        AccessModes cpuAccess = {};
        AccessModes gpuAccess = {};

        BindModes bind = {};
    };

}
