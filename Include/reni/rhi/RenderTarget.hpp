#ifndef RENI_RHI_RENDER_TARGET_HEADER
#define RENI_RHI_RENDER_TARGET_HEADER

#include "../utils.hpp"

namespace reni::rhi {

	class RenderTarget : private NonCopyable, private NonMovable {
	public:

		virtual ~RenderTarget() = default;

	};

}

#endif