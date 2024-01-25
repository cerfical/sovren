#include "RenderWindow.hpp"
#include "../DirectX/DxRenderDevice.hpp"

namespace RENI {
	void RenderWindow::onResize(Size2D newSize, Size2D oldSize) {
		Window::onResize(newSize, oldSize);

		m_renderThread.suspend();
		m_renderDevice->setSize(newSize);
		m_renderThread.resume();
	}

	
	RenderWindow::RenderWindow()
		: m_renderDevice(std::make_unique<DxRenderDevice>(static_cast<HWND>(nativeHandle())))
		, m_renderThread(this)
	{ }


	RenderDevice* RenderWindow::renderDevice() {
		return m_renderDevice.get();
	}

	RenderThread* RenderWindow::renderThread() {
		return &m_renderThread;
	}
}