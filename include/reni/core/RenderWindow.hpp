#pragma once

#include "Renderer.hpp"
#include "RenderGraph.hpp"
#include "Window.hpp"

namespace reni {

	class RenderWindow : public Window {
	public:

		RenderGraph& scene() {
			return m_scene;
		}


		void setFill(Color fill) {
			m_renderer.setClearColor(fill);
		}

		Color fill() const {
			return m_renderer.clearColor();
		}
		

	protected:
		void onResize(Size2 newSize) override {
			m_renderer.setRenderSize(newSize);
		}

		void onUpdate() override {
			m_renderer.renderScene(m_scene);
		}

	private:
		Renderer m_renderer = Renderer(*this);
		RenderGraph m_scene;
	};

}