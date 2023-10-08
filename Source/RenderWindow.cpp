#include "RenderWindow.hpp"
#include "RenderEngine.hpp"

namespace RENI {
	void RenderWindow::onResize(const Size2D& newSize, const Size2D& oldSize) {
		Window::onResize(newSize, oldSize);

		m_renderThread.suspend();
		m_renderDevice->resize(newSize);
		m_renderThread.resume();
	}

	
	RenderWindow::RenderWindow()
		: m_renderDevice(
			RenderEngine::get()->createWindowDevice(this)
		), m_renderThread(this)
	{ }


	RenderDevice* RenderWindow::renderDevice() {
		return m_renderDevice.get();
	}

	RenderThread* RenderWindow::renderThread() {
		return &m_renderThread;
	}
}