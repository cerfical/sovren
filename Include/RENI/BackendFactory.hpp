#ifndef RENI_BACKEND_FACTORY_HEADER
#define RENI_BACKEND_FACTORY_HEADER

#include "RenderBackend.hpp"
#include <memory>

namespace RENI {
	
	/**
	 * @brief Provides a platform abstraction layer.
	*/
	class BackendFactory {
	public:

		/**
		 * @brief Get a factory instance for the current platform.
		*/
		static BackendFactory* get();



		BackendFactory() = default;

		BackendFactory(const BackendFactory&) = delete;
		BackendFactory& operator=(const BackendFactory&) = delete;

		virtual ~BackendFactory() = default;



		/** @{ */
		/**
		 * @brief Create a renderer for a window.
		*/
		virtual std::unique_ptr<RenderBackend> createRender(void* window) = 0;
		/** @} */

	};

}

#endif