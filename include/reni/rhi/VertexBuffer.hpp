#pragma once

#include "../util/NonCopyable.hpp"
#include "../util/NonMovable.hpp"

namespace reni::rhi {
	
	class VertexBuffer : private NonCopyable, private NonMovable {
	public:
		virtual ~VertexBuffer() = 0;
	};

	inline VertexBuffer::~VertexBuffer() = default;

}