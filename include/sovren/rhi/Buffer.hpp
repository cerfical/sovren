#pragma once

namespace sovren::rhi {

    class Buffer {
    public:
        Buffer() = default;

        Buffer(const Buffer&) = delete;
        auto operator=(const Buffer&) -> Buffer& = delete;

        Buffer(Buffer&&) = delete;
        auto operator=(Buffer&&) -> Buffer& = delete;

        virtual ~Buffer() = 0;
    };

    inline Buffer::~Buffer() {}

}
