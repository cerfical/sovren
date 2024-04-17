#ifndef RENI_RHI_RENDER_CONTEXT_HEADER
#define RENI_RHI_RENDER_CONTEXT_HEADER

#include "../utils.hpp"

namespace reni::rhi {

	class CommandList;
	class RenderTarget;


	class RenderContext : private NonCopyable, private NonMovable {
	public:

		virtual ~RenderContext() = default;


		virtual void renderCommands(const CommandList& commands, RenderTarget& target) = 0;

	};

}

#endif