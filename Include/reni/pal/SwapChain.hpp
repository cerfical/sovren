#ifndef RENI_PAL_SWAP_CHAIN_HEADER
#define RENI_PAL_SWAP_CHAIN_HEADER

#include "../Size2D.hpp"
#include "../utils.hpp"

namespace reni::pal {

	class RenderTarget;


	class SwapChain : private NonCopyable, private NonMovable {
	public:

		virtual ~SwapChain() = default;


		virtual RenderTarget* frontBuffer() = 0;
	
		virtual void swapBuffers() = 0;
		

		virtual void setBufferSize(Size2D newSize) = 0;
		
		virtual Size2D bufferSize() const = 0;

	};

}

#endif