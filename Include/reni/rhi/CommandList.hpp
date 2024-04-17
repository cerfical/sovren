#ifndef RENI_RHI_COMMAND_LIST_HEADER
#define RENI_RHI_COMMAND_LIST_HEADER

#include "../Color.hpp"
#include "../Point2D.hpp"

#include "../utils.hpp"

namespace reni::rhi {

	class CommandList : private NonCopyable, private NonMovable {
	public:

		virtual ~CommandList() = default;


		virtual void startRender() = 0;
	
		virtual void endRender() = 0;


		virtual void drawLine(Point2D start, Point2D end) = 0;

		virtual void clear(Color clearColor) = 0;

	};

}

#endif