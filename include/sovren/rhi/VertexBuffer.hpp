#pragma once

namespace sovren::rhi {

    class VertexBuffer {
    public:
        VertexBuffer(const VertexBuffer&) = delete;
        VertexBuffer& operator=(const VertexBuffer&) = delete;

        VertexBuffer(VertexBuffer&&) = delete;
        VertexBuffer& operator=(VertexBuffer&&) = delete;

        VertexBuffer() = default;
        virtual ~VertexBuffer() = 0;
    };

    inline VertexBuffer::~VertexBuffer() = default;

}
