#include "SwapChain.hpp"
#include "RenderTarget.hpp"

namespace RENI {
	void SwapChain::clearBuffers(Color clearColor) {
		frontBuffer().clear(clearColor);
	}
}