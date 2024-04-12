#ifndef RENI_RENDER_HEADER
#define RENI_RENDER_HEADER

#include "util_types.hpp"

namespace reni {

	class RenderTarget;

	namespace rg {
		class RenderNode;
	}


	class Render : private NonCopyable, private NonMovable {
	public:

		virtual ~Render() = default;


		virtual void renderNode(const rg::RenderNode& n) = 0;

		virtual void startRender(RenderTarget& rt) = 0;
		virtual void endRender() = 0;

	};

}

#endif