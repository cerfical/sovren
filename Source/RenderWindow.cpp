#include "RenderWindow.hpp"
#include "BackendFactory.hpp"

namespace RENI {
	RenderWindow::RenderWindow()
		: m_render(BackendFactory::get()->createRender(nativeHandle())) {
		m_renderThread = std::jthread(&RenderWindow::renderLoop, this);
	}

	RenderWindow::~RenderWindow() {
		m_loopActive = false;
	}


	void RenderWindow::onResize(Size2D newSize, Size2D oldSize) {
		m_windowSize = newSize;
	}

	void RenderWindow::renderLoop() {
		auto buffersSize = m_windowSize.load();
		while(m_loopActive) {
			if(const auto windowSize = m_windowSize.load(); buffersSize != windowSize) {
				m_render->setBuffersSize(windowSize);
				buffersSize = windowSize;
			}

			m_render->clearBuffers(m_backgroundColor);
			{
				std::unique_lock lock(m_sceneMutex);
				m_render->drawScene(m_graphicsScene);
			}
		}
	}
}