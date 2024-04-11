#include "RenderWindow.hpp"

namespace RENI {
	RenderWindow::RenderWindow() {
		m_sceneRender = RenderBackend::get()->createRender(RenderType::Render2D);
		m_swapChain = RenderBackend::get()->createSwapChain(*this);

		m_renderThread = std::jthread(&RenderWindow::renderLoop, this);
	}

	RenderWindow::~RenderWindow() {
		m_loopActive = false;
	}


	void RenderWindow::onResize(Size2D newSize, Size2D oldSize) {
		m_windowSize = newSize;
	}


	void RenderWindow::renderLoop() {
		while(m_loopActive) {
			if(const auto windowSize = m_windowSize.load(); m_swapChain->bufferSize() != windowSize) {
				m_swapChain->setBufferSize(windowSize);
			}

			m_swapChain->clearBuffers(m_backgroundColor);
			{
				std::unique_lock lock(m_sceneMutex);
				m_sceneRender->startRender(m_swapChain->frontBuffer());
				for(const auto& n : m_graphicsScene.nodes()) {
						m_sceneRender->renderNode(*n);
				}
				m_sceneRender->endRender();
			}
			m_swapChain->swapBuffers();
		}
	}
}