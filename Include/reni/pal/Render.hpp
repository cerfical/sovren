#ifndef RENI_PAL_RENDER_HEADER
#define RENI_PAL_RENDER_HEADER

#include "../util_types.hpp"

namespace reni::rg {
	class RenderNode;
}

namespace reni::pal {

	class RenderTarget;


	enum class RenderType {
		Render2D
	};


	class Render : private NonCopyable, private NonMovable {
	public:

		virtual ~Render() = default;


		virtual void renderNode(const rg::RenderNode& n) = 0;

		virtual void startRender(RenderTarget& rt) = 0;
		virtual void endRender() = 0;

	};

}

#endif