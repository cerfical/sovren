#include "core/Renderer.hpp"

#include "pal/Platform.hpp"

#include "core/Window.hpp"
#include "core/RenderGraph.hpp"

#include "math.hpp"
#include "rhi.hpp"
#include "rg.hpp"

#include <unordered_map>

namespace reni {
	struct Renderer::Impl : private rg::NodeVisitor {

		void renderNodes(const rg::NodeList& n) {
			for(const auto& c : n) {
				c->accept(*this);
			}
		}


		std::unique_ptr<rhi::RenderBackend> renderApi;
		std::unique_ptr<rhi::SwapChain> swapChain;
		std::unique_ptr<rhi::RenderContext> renderContext;

		Window* targetWindow = {};
		Color clearColor = { 1.0f, 1.0f, 1.0f }; // clear the window with white color by default 


	private:
		void visit(const rg::Line2D& l) override {
			preRender2d();
			renderContext->drawLine(l.start(), l.end());
			postRender2d(l);
		}


		void visit(const rg::Rect2D& r) override {
			preRender2d();
			renderContext->drawRect(r.topLeft(), r.bottomRight());
			postRender2d(r);
		}


		void preRender2d() {
			if(m_transform2dChanged) {
				renderContext->setTransformMatrix(m_transform2d);
				m_transform2dChanged = false;
			}
		}


		void postRender2d(const rg::RenderNode& n) {
			renderNodes(n.children());
		}


		void visit(const rg::Transform2D& t) override {
			// make 2D geometry child nodes to render relative to their ancestors
			const auto oldTransform = std::exchange(m_transform2d, m_transform2d * t.matrix());
			m_transform2dChanged = true;

			renderNodes(t.children());

			m_transform2d = oldTransform;
			m_transform2dChanged = true;
		}


		void visit(const rg::Triangle3D& t) override {
			const auto [it, ok] = m_meshes.try_emplace(&t);
			auto& mesh = it->second;
			if(ok) {
				mesh = renderApi->createVertexBuffer(std::as_bytes(t.points()));
			}

			preRender3d();
			renderContext->drawMesh(*mesh);
			postRender3d(t);
		}


		void preRender3d() {
			if(m_transform3dChanged) {
				renderContext->setTransformMatrix(m_transform3d);
				m_transform3dChanged = false;
			}
		}


		void postRender3d(const rg::RenderNode& n) {
			renderNodes(n.children());
		}


		void visit(const rg::Transform3D& t) override {
			// make 3D geometry child nodes to render relative to their ancestors
			const auto oldTransform = std::exchange(m_transform3d, m_transform3d * t.matrix());
			m_transform3dChanged = true;

			renderNodes(t.children());
			
			m_transform3d = oldTransform;
			m_transform3dChanged = true;
		}


		std::unordered_map<const rg::RenderNode*, std::unique_ptr<rhi::VertexBuffer>> m_meshes;

		Mat3x3 m_transform2d = Mat3x3::identity();
		bool m_transform2dChanged = false;

		Mat4x4 m_transform3d = Mat4x4::identity();
		bool m_transform3dChanged = false;
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
		m_impl->renderContext->clear(m_impl->clearColor);
		
		const auto wndSize = m_impl->targetWindow->size();
		m_impl->renderContext->setProjectionMatrix({
			2.41421356245f / (static_cast<float>(wndSize.width) / wndSize.height), 0.0f, 0.0f, 0.0f,
			0.0f, 2.41421356245f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 1.0f,
			0.0f, 0.0f, -1.0f, 0.0f
		});

		m_impl->renderNodes(scene.nodes());

		m_impl->renderContext->endRender();
		m_impl->swapChain->swapBuffers();
	}


	void Renderer::setRenderSize(Size2 s) {
		m_impl->swapChain->setBufferSize(s);
	}


	void Renderer::setClearColor(Color clearCol) {
		m_impl->clearColor = clearCol;
	}


	Color Renderer::clearColor() const {
		return m_impl->clearColor;
	}


	Renderer::Renderer(Renderer&&) noexcept = default;
	Renderer& Renderer::operator=(Renderer&&) noexcept = default;

	Renderer::~Renderer() = default;
}