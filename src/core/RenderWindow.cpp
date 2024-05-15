#include "core/RenderWindow.hpp"

#include "math.hpp"
#include "pal.hpp"
#include "rhi.hpp"
#include "rg.hpp"

#include <unordered_map>

namespace reni {
	namespace {
		class SceneRenderer : private rg::NodeVisitor {
		public:
			
			SceneRenderer() = default;

			SceneRenderer(pal::Window& window)
				: m_window(&window) {

				m_renderApi = pal::Platform::get()->createRenderBackend();

				m_swapChain = m_renderApi->createSwapChain(window.nativeHandle());
				m_context = m_renderApi->createRenderContext();
			}


			void render(const RenderGraph& scene, Color clear) {
				m_context->startRender(m_swapChain->frontBuffer());
				m_context->clear(clear);
				
				const auto wndSize = m_window->getClientSize();
				m_context->setProjectionMatrix({
					2.41421356245f / (static_cast<float>(wndSize.width) / wndSize.height), 0.0f, 0.0f, 0.0f,
					0.0f, 2.41421356245f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 1.0f,
					0.0f, 0.0f, -1.0f, 0.0f
				});

				for(const auto& n : scene.nodes()) {
					n->accept(*this);
				}

				m_context->endRender();
				m_swapChain->swapBuffers();
			}


			void setRenderSize(Size2 s) {
				m_swapChain->setBufferSize(s);
			}


		private:
			void visit(const rg::Line2D& l) override {
				m_context->drawLine(l.start(), l.end());
			}

			void visit(const rg::Rect2D& r) override {
				m_context->drawRect(r.topLeft(), r.botRight());
			}

			void visit(const rg::Transform2D& t) override {
				const auto old = std::exchange(m_transform2d, m_transform2d * t.matrix());

				// render child nodes representing some 2D geometry relative to their parent				
				m_context->setTransformMatrix(m_transform2d);
				for(const auto& c : t.children()) {
					c->accept(*this);
				}

				m_context->setTransformMatrix(old);
				m_transform2d = old;
			}

			void visit(const rg::Triangle3D& t) override {
				const auto [it, ok] = m_meshes.try_emplace(&t);
				auto& mesh = it->second;

				if(ok) {
					mesh = m_renderApi->createVertexBuffer(std::as_bytes(t.points()));
				}
				m_context->drawMesh(*mesh);
			}

			void visit(const rg::Transform3D& t) override {
				const auto old = std::exchange(m_transform3d, m_transform3d * t.matrix());

				// render child nodes representing some 3D geometry relative to their parent				
				m_context->setTransformMatrix(m_transform3d);
				for(const auto& c : t.children()) {
					c->accept(*this);
				}

				m_context->setTransformMatrix(old);
				m_transform3d = old;
			}

		private:
			std::unordered_map<const rg::RenderNode*, std::unique_ptr<rhi::VertexBuffer>> m_meshes;

			std::unique_ptr<rhi::RenderBackend> m_renderApi;
			std::unique_ptr<rhi::SwapChain> m_swapChain;
			std::unique_ptr<rhi::RenderContext> m_context;

			Mat3x3 m_transform2d = Mat3x3::identity();
			Mat4x4 m_transform3d = Mat4x4::identity();

			pal::Window* m_window = {};
		};
	}


	struct RenderWindow::Impl : public pal::WindowCallbacks {
		
		void onWindowClose() override {
			palWindow->setVisible(false);
			visible = false;
		}


		void onWindowResize(Size2 newSize) override {
			if(newSize != clientSize) {
				renderer.setRenderSize(newSize);
				renderWindow->onResize(newSize);
				clientSize = newSize;
			}
		}


		void onKeyStateChange(Keys key, bool pressed) override {
			if(pressed) {
				renderWindow->onKeyDown(key);
			} else {
				renderWindow->onKeyUp(key);
			}
		}


		void onMouseButtonStateChange(MouseButtons button, bool pressed) override {
			if(pressed) {
				renderWindow->onButtonDown(button);
			} else {
				renderWindow->onButtonUp(button);
			}
		}


		void onMouseMove(Point2 newPos) override {
			if(newPos != mousePos) {
				renderWindow->onMouseMove(newPos);
				mousePos = newPos;
			}
		}


		std::unique_ptr<pal::Window> palWindow;
		RenderWindow* renderWindow = {};

		SceneRenderer renderer;
		RenderGraph scene;

		std::string title;
		Size2 clientSize;
		Point2 mousePos;
		bool visible = false;

		// clear the window with white color by default 	
		Color fill = { 1.0f, 1.0f, 1.0f };
	};


	RenderWindow::RenderWindow()
		: m_impl(std::make_unique<Impl>()) {

		auto platform = pal::Platform::get();
		m_impl->palWindow = platform->createWindow();

		m_impl->renderer = SceneRenderer(*m_impl->palWindow);

		m_impl->clientSize = m_impl->palWindow->getClientSize();
		m_impl->mousePos = m_impl->palWindow->getMousePos();

		m_impl->palWindow->installCallbacks(m_impl.get());
		m_impl->renderWindow = this;
	}


	void RenderWindow::show() {
		onShow();

		m_impl->palWindow->setVisible(true);
		m_impl->visible = true;

		auto eventPoller = pal::Platform::get()->createEventPoller();
		while(m_impl->visible) {
			eventPoller->pollEvents();
			onUpdate();

			m_impl->renderer.render(m_impl->scene, m_impl->fill);
		}

		onHide();
	}


	void RenderWindow::setTitle(const std::string& newTitle) {
		m_impl->palWindow->setTitle(newTitle);
		m_impl->title = newTitle;
	}


	const std::string& RenderWindow::title() const {
		return m_impl->title;
	}


	void RenderWindow::setSize(Size2 newSize) {
		m_impl->palWindow->setClientSize(newSize);
	}


	Size2 RenderWindow::size() const {
		return m_impl->clientSize;
	}


	Point2 RenderWindow::mousePos() const {
		return m_impl->mousePos;
	}


	RenderGraph& RenderWindow::scene() {
		return m_impl->scene;
	}


	void RenderWindow::setFill(Color fill) {
		m_impl->fill = fill;
	}


	Color RenderWindow::fill() const {
		return m_impl->fill;
	}


	RenderWindow::RenderWindow(RenderWindow&&) noexcept = default;
	RenderWindow& RenderWindow::operator=(RenderWindow&&) noexcept = default;

	RenderWindow::~RenderWindow() = default;


	void RenderWindow::onResize(Size2) {}

	void RenderWindow::onShow() {}
	void RenderWindow::onUpdate() {}
	void RenderWindow::onHide() {}

	void RenderWindow::onKeyDown(Keys) {}
	void RenderWindow::onKeyUp(Keys) {}

	void RenderWindow::onButtonDown(MouseButtons) {}
	void RenderWindow::onButtonUp(MouseButtons) {}
	void RenderWindow::onMouseMove(Point2) {}
}