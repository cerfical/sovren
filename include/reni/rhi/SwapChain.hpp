#pragma once

#include "../util/NonCopyable.hpp"
#include "../util/NonMovable.hpp"
#include "../util/Size2.hpp"

namespace reni::rhi {

	class RenderTarget;


	class SwapChain : private NonCopyable, private NonMovable {
	public:

		virtual ~SwapChain() = default;


		virtual RenderTarget& frontBuffer() = 0;
	
		virtual void swapBuffers() = 0;
		

		virtual void setBufferSize(Size2 newSize) = 0;
		
		virtual Size2 bufferSize() const = 0;

	};

}