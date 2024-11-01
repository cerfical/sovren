#pragma once

namespace sovren::rhi {

    class VertexBuffer {
    public:
        VertexBuffer() = default;

        VertexBuffer(const VertexBuffer&) = delete;
        auto operator=(const VertexBuffer&) -> VertexBuffer& = delete;

        VertexBuffer(VertexBuffer&&) = delete;
        auto operator=(VertexBuffer&&) -> VertexBuffer& = delete;

        virtual ~VertexBuffer() = 0;
    };

    inline VertexBuffer::~VertexBuffer() {}

}
