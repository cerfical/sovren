#ifndef RENI_RHI_RENDER_TARGET_HEADER
#define RENI_RHI_RENDER_TARGET_HEADER

#include "../util/types.hpp"

namespace reni::rhi {

	class RenderTarget : private NonCopyable, private NonMovable {
	public:

		virtual ~RenderTarget() = default;

	};

}

#endif