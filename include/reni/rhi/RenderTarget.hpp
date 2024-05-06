#pragma once

#include "../util/types.hpp"

namespace reni::rhi {

	class RenderTarget : private NonCopyable, private NonMovable {
	public:

		virtual ~RenderTarget() = default;

	};

}