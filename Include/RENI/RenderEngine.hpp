#ifndef RENI_RENDER_ENGINE_HEADER
#define RENI_RENDER_ENGINE_HEADER

#include "RenderDevice.hpp"
#include <memory>

namespace RENI {
	/**
	 * @brief Unified access to the rendering API.
	 */
	class RenderEngine {
	public:
		/** @{ */
		/** @brief Get the platform-default RenderEngine implementation. */
		static RenderEngine* get();
		/** @} */

		/** @{ */
		/** @brief Destroy the engine, releasing all resources. */
		virtual ~RenderEngine() = default;
		/** @} */

		/** @{ */
		/** @brief Create a RenderDevice to render to the specified window. */
		virtual std::unique_ptr<RenderDevice> createWindowDevice(void* window) = 0;
		/** @} */
	};
}

#endif