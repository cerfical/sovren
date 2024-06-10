#pragma once

#include "../util/NonCopyable.hpp"
#include "../util/NonMovable.hpp"

namespace reni::rhi {

	class RenderTarget : private NonCopyable, private NonMovable {
	public:
		virtual ~RenderTarget() = 0;
	};

	inline RenderTarget::~RenderTarget() = default;

}