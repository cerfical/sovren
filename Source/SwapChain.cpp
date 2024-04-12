#include "SwapChain.hpp"
#include "RenderTarget.hpp"

namespace reni {
	void SwapChain::clearBuffers(Color clearColor) {
		frontBuffer().clear(clearColor);
	}
}