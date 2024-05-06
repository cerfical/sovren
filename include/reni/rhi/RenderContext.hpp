#pragma once

#include "../util/types.hpp"

namespace reni::rhi {

	class CommandList;
	class RenderTarget;


	class RenderContext : private NonCopyable, private NonMovable {
	public:

		virtual ~RenderContext() = default;


		virtual void renderCommands(const CommandList& commands, RenderTarget& target) = 0;

	};

}