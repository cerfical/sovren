#ifndef RENI_DX_RENDER_ENGINE_HEADER
#define RENI_DX_RENDER_ENGINE_HEADER

#include "RenderEngine.hpp"
#include "DxUtils.hpp"

namespace RENI {
	/**
	 * @brief Direct3D RenderEngine implementation.
	 */
	class DxRenderEngine : public RenderEngine {
	public:
		/** @{ */
		DxRenderEngine();
		/** @} */

		/** @{ */
		std::unique_ptr<RenderDevice> createWindowDevice(Window* window) override;
		/** @{ */

	private:
		/** @{ */
		ComPtr<ID3D11Device> m_d3dDevice;
		ComPtr<ID3D11DeviceContext> m_d3dContext;
		/** @} */
	};
}

#endif