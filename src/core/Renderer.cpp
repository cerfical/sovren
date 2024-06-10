#include "core/Renderer.hpp"

#include "core/RenderGraph.hpp"
#include "core/Window.hpp"

#include "math/Mat3x3.hpp"
#include "math/Mat4x4.hpp" 

#include "pal/Platform.hpp"

#include "rg/Camera3D.hpp"
#include "rg/Line2D.hpp"
#include "rg/NodeVisitor.hpp"
#include "rg/Rect2D.hpp"
#include "rg/RenderNode.hpp"
#include "rg/Transform2D.hpp"
#include "rg/Transform3D.hpp"
#include "rg/Triangle3D.hpp"

#include "rhi/RenderBackend.hpp"
#include "rhi/RenderContext.hpp"
#include "rhi/SwapChain.hpp"

#include <stack>
#include <unordered_map>

namespace reni {
	struct Renderer::Impl : private rg::NodeVisitor {

		void renderScene(const RenderGraph& scene) {
			// start with no-op transformations
			m_transformStack2d.push(Mat3x3::identity());
			m_transformStack3d.push(Mat4x4::identity());
			m_viewProjStack.push(Mat4x4::identity());

			visitNodes(scene.nodes());

			m_viewProjStack.pop();
			m_transformStack3d.pop();
			m_transformStack2d.pop();
		}


		std::unique_ptr<rhi::RenderBackend> renderApi;
		std::unique_ptr<rhi::RenderContext> renderContext;
		std::unique_ptr<rhi::SwapChain> swapChain;

		Color clearColor = { 1.0f, 1.0f, 1.0f }; // clear the window with white color by default 


	private:
		void visit(const rg::Line2D& l) override {
			setupRender2d();
			renderContext->drawLine(l.start(), l.end());
			
			visitChildren(l);
		}

		void visit(const rg::Rect2D& r) override {
			setupRender2d();
			renderContext->drawRect(r.topLeft(), r.bottomRight());
			
			visitChildren(r);
		}

		void setupRender2d() {
			renderContext->setTransformMatrix(m_transformStack2d.top());
		}

		void visit(const rg::Transform2D& t) override {
			// make 2D geometry child nodes to render relative to their parent
			m_transformStack2d.push(m_transformStack2d.top() * t.toMatrix());
			visitChildren(t);
			m_transformStack2d.pop();
		}


		void visit(const rg::Triangle3D& t) override {
			const auto [it, ok] = m_meshes.try_emplace(&t);
			auto& mesh = it->second;
			if(ok) {
				mesh = renderApi->createVertexBuffer(std::as_bytes(t.points()));
			}

			setupRender3d();
			renderContext->drawMesh(*mesh);
			visitChildren(t);
		}
		
		void setupRender3d() {
			renderContext->setTransformMatrix(m_transformStack3d.top() * m_viewProjStack.top());
		}

		void visit(const rg::Transform3D& t) override {
			// make 3D geometry child nodes to render relative to their parent
			m_transformStack3d.push(m_transformStack3d.top() * t.toMatrix());
			visitChildren(t);
			m_transformStack3d.pop();
		}

		void visit(const rg::Camera3D& c) override {
			m_viewProjStack.push(
				m_transformStack3d.top().inverted() * // calculate the camera view matrix based on position in the scene
				c.toProjMatrix() // then apply the projection matrix
			);

			// make all child nodes to be positioned independently of the camera
			m_transformStack3d.push(Mat4x4::identity());

			visitChildren(c);

			m_transformStack3d.pop();
			m_viewProjStack.pop();
		}


		void visitChildren(const rg::RenderNode& n) {
			visitNodes(n.children());
		}

		void visitNodes(const rg::NodeList& n) {
			for(const auto& c : n) {
				c->accept(*this);
			}
		}


		std::unordered_map<const rg::RenderNode*, std::unique_ptr<rhi::VertexBuffer>> m_meshes;

		std::stack<Mat3x3> m_transformStack2d;
		std::stack<Mat4x4> m_transformStack3d;
		std::stack<Mat4x4> m_viewProjStack;
	};


	Renderer::Renderer(Window& window)
		: m_impl(std::make_unique<Impl>()) {

		m_impl->renderApi = pal::Platform::get()->createRenderBackend();
		m_impl->swapChain = m_impl->renderApi->createSwapChain(window.nativeHandle());
		m_impl->renderContext = m_impl->renderApi->createRenderContext();
	}


	void Renderer::renderScene(const RenderGraph& scene) {
		m_impl->renderContext->startRender(m_impl->swapChain->frontBuffer());
		m_impl->renderContext->clear(m_impl->clearColor);
		
		m_impl->renderScene(scene);

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