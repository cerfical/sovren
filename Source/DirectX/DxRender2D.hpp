#ifndef RENI_DX_RENDER_2D_HEADER
#define RENI_DX_RENDER_2D_HEADER

#include "DxUtils.hpp"
#include "rg.hpp"

namespace RENI {

	class DxRender2D : rg::NodeVisitor {
	public:

		DxRender2D() {
			comCheck(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, factoryOptions, &m_factory));
		}

		DxRender2D(const DxRender2D&) = delete;
		DxRender2D& operator=(const DxRender2D&) = delete;



		void setRenderTarget(IDXGISurface* rt) {
			if(rt) {
				comCheck(m_factory->CreateDxgiSurfaceRenderTarget(
					rt, D2D1::RenderTargetProperties(
						D2D1_RENDER_TARGET_TYPE_HARDWARE,
						D2D1::PixelFormat(
							DXGI_FORMAT_UNKNOWN,
							D2D1_ALPHA_MODE_IGNORE
						)
					), &m_renderTarget
				));
			} else {
				m_renderTarget = nullptr;
			}

			if(!m_drawBrush && m_renderTarget) {
				comCheck(m_renderTarget->CreateSolidColorBrush(drawColor, &m_drawBrush));
			}
		}

		void clearRenderTarget(Color c) {
			startRender();
			m_renderTarget->Clear(toColorF(c));
		}



		void startRender() {
			if(!m_renderStarted) {
				m_renderTarget->BeginDraw();
				m_renderStarted = true;
			}
		}

		void endRender() {
			if(m_renderStarted) {
				comCheck(m_renderTarget->EndDraw());
				m_renderStarted = false;
			}
		}

		void render(const rg::RenderNode& n) {
			n.acceptVisitor(*this);
		}



	private:
		constexpr static D2D1_FACTORY_OPTIONS factoryOptions = {
#if not defined(NDEBUG) || defined(_DEBUG)
			D2D1_DEBUG_LEVEL_INFORMATION
#else
			D2D1_DEBUG_LEVEL_NONE
#endif
		};
		const inline static D2D1::ColorF drawColor = { D2D1::ColorF::Black };



		void visit(const rg::Line2D& n) override {
			m_renderTarget->DrawLine(toPoint2F(n.start), toPoint2F(n.end), m_drawBrush);
		}

		void visit(const rg::Rect2D& n) override {
			const auto tl = toPoint2F(n.topLeft), br = toPoint2F(n.bottomRight);
			m_renderTarget->DrawRectangle({
					.left = tl.x, .top = tl.y, .right = br.x, .bottom = br.y
				}, m_drawBrush
			);
		}



		ComPtr<ID2D1RenderTarget> m_renderTarget;
		ComPtr<ID2D1SolidColorBrush> m_drawBrush;
		ComPtr<ID2D1Factory> m_factory;

		bool m_renderStarted = false;
	};

}

#endif