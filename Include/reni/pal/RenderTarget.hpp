#ifndef RENI_PAL_RENDER_TARGET_HEADER
#define RENI_PAL_RENDER_TARGET_HEADER

#include "../utils.hpp"
#include "../Color.hpp"

namespace reni::pal {

	class RenderTarget : private NonCopyable, private NonMovable {
	public:

		virtual ~RenderTarget() = default;

		virtual void clear(Color clearColor) = 0;

	};

}

#endif