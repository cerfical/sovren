#ifndef RENI_RENDER_WINDOW_HEADER
#define RENI_RENDER_WINDOW_HEADER

#include "Color.hpp"
#include "RenderBackend.hpp"
#include "RenderGraph.hpp"
#include "LockedPtr.hpp"
#include "Window.hpp"
#include "Render.hpp"
#include "SwapChain.hpp"

#include <thread>
#include <mutex>

namespace RENI {

	/**
	 * @brief Extension of Window for rendering 2D/3D graphics.
	*/
	class RenderWindow : public Window {
	public:
		
		RenderWindow();
		~RenderWindow() override;

		RenderWindow(const RenderWindow&) = delete;
		RenderWindow& operator=(const RenderWindow&) = delete;



		/** @{ */
		/**
		 * @brief Set a new background color for the window.
		*/
		void setBackgroundColor(Color c) {
			m_backgroundColor = c;
		}



		/**
		 * @brief Background color of the window.
		*/
		Color backgroundColor() const {
			return m_backgroundColor;
		}



		/**
		 * @brief Read-only description of the graphical objects to be displayed in the window.
		*/
		const RenderGraph* graphicsScene() const {
			return &m_graphicsScene;
		}



		/**
		 * @brief Get exclusive write access to the graphics scene.
		*/
		LockedPtr<RenderGraph> lockGraphicsScene() {
			return LockedPtr(&m_graphicsScene, m_sceneMutex);
		}
		/** @} */
		


	private:
		void onResize(Size2D newSize, Size2D oldSize) override;
		void renderLoop();

		std::atomic<Size2D> m_windowSize = size();

		std::atomic<Color> m_backgroundColor;
		std::atomic_bool m_loopActive = true;
		
		std::unique_ptr<Render> m_sceneRender;
		std::unique_ptr<SwapChain> m_swapChain;
		RenderGraph m_graphicsScene;
		std::mutex m_sceneMutex;

		std::jthread m_renderThread;
	};

}

#endif