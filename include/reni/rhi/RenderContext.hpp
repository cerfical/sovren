#pragma once

#include "../util/types.hpp"

namespace reni::rhi {

	class RenderTarget;


	class RenderContext : private NonCopyable, private NonMovable {
	public:

		virtual ~RenderContext() = default;


		virtual void startRender(RenderTarget& rt) = 0;
	
		virtual void endRender() = 0;


		virtual void drawLine(Point2 start, Point2 end) = 0;

		virtual void drawRect(Point2 topLeft, Point2 botRight) = 0;

		virtual void clear(Color clearColor) = 0;

	};

}