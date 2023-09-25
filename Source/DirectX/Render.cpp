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
	Render::Context& Render::Context::SetDrawColor(Color c) {
		m_impl->render.SetDrawColor(c);
		return *this;
	}

	Render::Context& Render::Context::Clear(Color c) {
		m_impl->render.Clear(c);
		return *this;
	}


	Render::Context& Render::Context::DrawLine(const Line2D& l) {
		m_impl->render.DrawLine(l);
		return *this;
	}

	Render::Context& Render::Context::DrawRect(const Rect2D& r) {
		m_impl->render.DrawRect(r);
		return *this;
	}
	
	Render::Context& Render::Context::FillRect(const Rect2D& r) {
		m_impl->render.FillRect(r);
		return *this;
	}


	void Render::Context::Present() {
		if(!std::exchange(m_presented, true)) {
			m_impl->render.EndRender();
		}
	}
}

namespace RENI {
	Render::Render(Window& window) {
		m_impl = std::make_unique<Impl>(window);
	}
	
	Render::~Render() = default;


	Render::Context Render::StartRender() {
		m_impl->render.StartRender();
		return Context(*m_impl);
	}
}