#ifndef RENI_SWAP_CHAIN_HEADER
#define RENI_SWAP_CHAIN_HEADER

#include "util_types.hpp"

#include "Size2D.hpp"
#include "Color.hpp"

namespace reni {

	class RenderTarget;


	class SwapChain : private NonCopyable, private NonMovable {
	public:

		virtual ~SwapChain() = default;


		virtual void swapBuffers() = 0;
		
		void clearBuffers(Color clearColor);


		virtual void setBufferSize(Size2D s) = 0;
		
		virtual Size2D bufferSize() const = 0;


		virtual RenderTarget& frontBuffer() = 0;

	};

}

#endif