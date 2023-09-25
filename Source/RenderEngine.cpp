#include "DxRenderEngine.hpp"

namespace RENI {
	RenderEngine* RenderEngine::get() {
		static DxRenderEngine renderEngine;
		return &renderEngine;
	}
}