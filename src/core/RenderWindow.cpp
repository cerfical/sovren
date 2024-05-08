#include "core/RenderWindow.hpp"

#include "pal.hpp"
#include "rhi.hpp"
#include "rg.hpp"

namespace reni {
	namespace {
		class SceneRenderer : private rg::NodeVisitor {
		public:
			
			SceneRenderer() = default;

			SceneRenderer(pal::Window& window) {
				auto renderApi = pal::Platform::get()->createRenderBackend();

				m_swapChain = renderApi->createSwapChain(window.nativeHandle());
				m_context = renderApi->createRenderContext();
			}


			void render(const RenderGraph& scene, Color clearColor) {
				m_context->startRender(m_swapChain->frontBuffer());
				m_context->clear(clearColor);
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
				m_context->drawLine(l.start, l.end);
			}

			void visit(const rg::Rect2D& r) override {
				const auto bottomRight = Point2(r.topLeft.x + r.size.width, r.topLeft.y + r.size.height);
				m_context->drawRect(r.topLeft, bottomRight);
			}

		private:
			std::unique_ptr<rhi::SwapChain> m_swapChain;
			std::unique_ptr<rhi::RenderContext> m_context;
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
				renderWindow->onResize(newSize, std::exchange(clientSize, newSize));
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
				renderWindow->onMouseMove(std::exchange(mousePos, newPos), newPos);
			}
		}


		std::unique_ptr<pal::Window> palWindow;
		RenderWindow* renderWindow = {};

		SceneRenderer renderer;
		RenderGraph scene;

		std::string title;
		Size2 clientSize;
		Point2 mousePos;
		Color fillColor;

		bool visible = false;
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

			m_impl->renderer.render(m_impl->scene, m_impl->fillColor);
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


	void RenderWindow::setFillColor(Color fillColor) {
		m_impl->fillColor = fillColor;
	}


	Color RenderWindow::fillColor() const {
		return m_impl->fillColor;
	}


	RenderWindow::RenderWindow(RenderWindow&&) noexcept = default;
	RenderWindow& RenderWindow::operator=(RenderWindow&&) noexcept = default;

	RenderWindow::~RenderWindow() = default;
}