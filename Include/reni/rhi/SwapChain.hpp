#ifndef RENI_RHI_SWAP_CHAIN_HEADER
#define RENI_RHI_SWAP_CHAIN_HEADER

#include "../util/types.hpp"

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

#endif