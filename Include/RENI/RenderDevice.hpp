#ifndef RENI_RENDER_DEVICE_HEADER
#define RENI_RENDER_DEVICE_HEADER

#include "RenderGraph.hpp"
#include "Size2D.hpp"
#include "Color.hpp"

namespace RENI {

	/**
	 * @brief Two-dimensional surface for performing 2D/3D rendering operations.
	*/
	class RenderDevice {
	public:

		/** @{ */
		/** Destroy the device. */
		virtual ~RenderDevice() = default;
		/** @} */


		/** @{ */
		/** @brief Set the new size of the buffers backing the device. */
		virtual void setSize(Size2D s) = 0;

		/** @brief Current size of the device buffers. */
		virtual Size2D size() const = 0;

		/** @brief Current width of the device buffers. */
		int width() const {
			return size().width;
		}

		/** @brief Current height of the device buffers. */
		int height() const {
			return size().height;
		}
		/** @} */


		/** @{ */
		virtual void drawScene(const RenderGraph& scene) = 0;

		/** @brief Fill the device surface with the specified color. */
		virtual void clear(Color c) = 0;
		/** @} */

	};

}

#endif