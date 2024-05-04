#ifndef RENI_RHI_COMMAND_LIST_HEADER
#define RENI_RHI_COMMAND_LIST_HEADER

#include "../util/types.hpp"

namespace reni::rhi {

	class CommandList : private NonCopyable, private NonMovable {
	public:

		virtual ~CommandList() = default;


		virtual void startRender() = 0;
	
		virtual void endRender() = 0;


		virtual void drawLine(Point2 start, Point2 end) = 0;

		virtual void drawRect(Point2 topLeft, Point2 bottomRight) = 0;

		virtual void clear(Color clearColor) = 0;

	};

}

#endif