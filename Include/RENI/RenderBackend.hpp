#ifndef RENI_RENDER_BACKEND_HEADER
#define RENI_RENDER_BACKEND_HEADER

#include "Size2D.hpp"
#include "Color.hpp"

namespace RENI {
	class RenderGraph;

	/**
	 * @brief Abstracts away different rendering APIs.
	*/
	class RenderBackend {
	public:

		RenderBackend() = default;

		RenderBackend(const RenderBackend&) = delete;
		RenderBackend& operator=(const RenderBackend&) = delete;

		virtual ~RenderBackend() = default;



		/** @{ */
		/**
		 * @brief Set a new size for the rendering buffers.
		*/
		virtual void setBuffersSize(Size2D s) = 0;



		/**
		 * @brief Fill the rendering buffers with a color.
		*/
		virtual void clearBuffers(Color c) = 0;



		/**
		 * @brief Render a scene graph into the buffers.
		*/
		virtual void drawScene(const RenderGraph& scene) = 0;
		/** @} */

	};

}

#endif