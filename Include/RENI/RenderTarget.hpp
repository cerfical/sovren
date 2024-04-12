#ifndef RENI_RENDER_TARGET_HEADER
#define RENI_RENDER_TARGET_HEADER

#include "util_types.hpp"
#include "Color.hpp"

namespace reni {

	class RenderTarget : private NonCopyable, private NonMovable {
	public:

		virtual ~RenderTarget() = default;

		virtual void clear(Color clearColor) = 0;

	};

}

#endif