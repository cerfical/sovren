#include "core/Renderer.hpp"

#include "pal/Platform.hpp"

#include "core/Window.hpp"
#include "core/RenderGraph.hpp"

#include "math.hpp"
#include "rhi.hpp"
#include "rg.hpp"

#include <unordered_map>

namespace reni {
	struct Renderer::Impl : public rg::NodeVisitor {

		void visit(const rg::Line2D& l) override {
			renderContext->drawLine(l.start(), l.end());
		}


		void visit(const rg::Rect2D& r) override {
			renderContext->drawRect(r.topLeft(), r.bottomRight());
		}


		void visit(const rg::Transform2D& t) override {
			const auto old = std::exchange(transform2d, transform2d * t.matrix());

			// render child nodes representing some 2D geometry relative to their parent				
			renderContext->setTransformMatrix(transform2d);
			for(const auto& c : t.children()) {
				c->accept(*this);
			}

			renderContext->setTransformMatrix(old);
			transform2d = old;
		}


		void visit(const rg::Triangle3D& t) override {
			const auto [it, ok] = meshes.try_emplace(&t);
			auto& mesh = it->second;

			if(ok) {
				mesh = renderApi->createVertexBuffer(std::as_bytes(t.points()));
			}
			renderContext->drawMesh(*mesh);
		}


		void visit(const rg::Transform3D& t) override {
			const auto old = std::exchange(transform3d, transform3d * t.matrix());

			// render child nodes representing some 3D geometry relative to their parent				
			renderContext->setTransformMatrix(transform3d);
			for(const auto& c : t.children()) {
				c->accept(*this);
			}

			renderContext->setTransformMatrix(old);
			transform3d = old;
		}


		std::unordered_map<const rg::RenderNode*, std::unique_ptr<rhi::VertexBuffer>> meshes;

		std::unique_ptr<rhi::RenderBackend> renderApi;
		std::unique_ptr<rhi::SwapChain> swapChain;
		std::unique_ptr<rhi::RenderContext> renderContext;

		Mat3x3 transform2d = Mat3x3::identity();
		Mat4x4 transform3d = Mat4x4::identity();

		Window* targetWindow = {};
		Color clearCol = { 1.0f, 1.0f, 1.0f }; // clear the window with white color by default 	
	};


	Renderer::Renderer(Window& window)
		: m_impl(std::make_unique<Impl>()) {

		m_impl->renderApi = pal::Platform::get()->createRenderBackend();
		m_impl->swapChain = m_impl->renderApi->createSwapChain(window.nativeHandle());
		m_impl->renderContext = m_impl->renderApi->createRenderContext();

		m_impl->targetWindow = &window;
	}


	void Renderer::renderScene(const RenderGraph& scene) {
		m_impl->renderContext->startRender(m_impl->swapChain->frontBuffer());
		m_impl->renderContext->clear(m_impl->clearCol);
		
		const auto wndSize = m_impl->targetWindow->size();
		m_impl->renderContext->setProjectionMatrix({
			2.41421356245f / (static_cast<float>(wndSize.width) / wndSize.height), 0.0f, 0.0f, 0.0f,
			0.0f, 2.41421356245f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 1.0f,
			0.0f, 0.0f, -1.0f, 0.0f
		});

		for(const auto& n : scene.nodes()) {
			n->accept(*m_impl);
		}

		m_impl->renderContext->endRender();
		m_impl->swapChain->swapBuffers();
	}


	void Renderer::setRenderSize(Size2 s) {
		m_impl->swapChain->setBufferSize(s);
	}


	void Renderer::setClearColor(Color clearCol) {
		m_impl->clearCol = clearCol;
	}


	Color Renderer::clearColor() const {
		return m_impl->clearCol;
	}


	Renderer::Renderer(Renderer&&) noexcept = default;
	Renderer& Renderer::operator=(Renderer&&) noexcept = default;

	Renderer::~Renderer() = default;
}