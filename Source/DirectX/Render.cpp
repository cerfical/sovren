#include "Render.hpp"
#include "D3DRender.hpp"

struct RENI::Render::Impl {
	/** @{ */
	explicit Impl(Window& window)
		: render(window)
	{ }
	/** @} */

	D3DRender render;
};

namespace RENI {
	Render::Context& Render::Context::setDrawColor(Color c) {
		m_impl->render.setDrawColor(c);
		return *this;
	}

	Render::Context& Render::Context::clear(Color c) {
		m_impl->render.clear(c);
		return *this;
	}


	Render::Context& Render::Context::drawLine(const Line2D& l) {
		m_impl->render.drawLine(l);
		return *this;
	}

	Render::Context& Render::Context::drawRect(const Rect2D& r) {
		m_impl->render.drawRect(r);
		return *this;
	}
	
	Render::Context& Render::Context::fillRect(const Rect2D& r) {
		m_impl->render.fillRect(r);
		return *this;
	}


	void Render::Context::present() {
		if(!std::exchange(m_presented, true)) {
			m_impl->render.endRender();
		}
	}
}

namespace RENI {
	Render::Render(Window& window) {
		m_impl = std::make_unique<Impl>(window);
	}
	
	Render::~Render() = default;


	Render::Context Render::startRender() {
		m_impl->render.startRender();
		return Context(*m_impl);
	}
}