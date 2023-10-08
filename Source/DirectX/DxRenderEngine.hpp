#ifndef RENI_DX_RENDER_ENGINE_HEADER
#define RENI_DX_RENDER_ENGINE_HEADER

#include "RenderEngine.hpp"

namespace RENI {
	/**
	 * @brief DirectX RenderEngine implementation.
	 */
	class DxRenderEngine : public RenderEngine {
	public:
		/** @{ */
		DxRenderEngine() = default;
		~DxRenderEngine() override = default;
		/** @} */


		/** @{ */
		DxRenderEngine(DxRenderEngine&&) = default;
		DxRenderEngine& operator=(DxRenderEngine&&) = default;
		/** @} */

		/** @{ */
		DxRenderEngine(const DxRenderEngine&) = delete;
		DxRenderEngine& operator=(const DxRenderEngine&) = delete;
		/** @} */


		/** @{ */
		std::unique_ptr<RenderDevice> createWindowDevice(Window* window) override;
		/** @} */
	};
}

#endif