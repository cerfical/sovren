#pragma once

#include "../util/types.hpp"

namespace reni::rhi {
	
	class VertexBuffer : private NonCopyable, private NonMovable {
	public:
		virtual ~VertexBuffer() = 0;
	};

	inline VertexBuffer::~VertexBuffer() = default;

}