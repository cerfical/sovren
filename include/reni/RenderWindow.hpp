#ifndef RENI_RENDER_WINDOW_HEADER
#define RENI_RENDER_WINDOW_HEADER

#include "Keys.hpp"
#include "MouseButtons.hpp"
#include "RenderGraph.hpp"

#include "util/types.hpp"

#include <string>
#include <memory>

namespace reni {

	class RenderWindow : private NonCopyable {
	public:

		RenderWindow();
		
		~RenderWindow();


		RenderWindow(RenderWindow&&) noexcept;
		RenderWindow& operator=(RenderWindow&&) noexcept;


		void setTitle(const std::string& newTitle);

		const std::string& title() const;


		void setSize(Size2 newSize);
		
		Size2 size() const;


		Point2 mousePos() const;


		RenderGraph& scene();

		void setFillColor(Color fillColor);

		Color fillColor() const;


		void show();


	protected:
		virtual void onResize(Size2 newSize, Size2 oldSize) {}
		
		virtual void onShow() {}
		virtual void onUpdate() {}
		virtual void onHide() {}

		virtual void onKeyDown(Keys pressedKey) {}
		virtual void onKeyUp(Keys releasedKey) {}

		virtual void onButtonDown(MouseButtons pressedButton) {}
		virtual void onButtonUp(MouseButtons releasedButton) {}
		virtual void onMouseMove(Point2 newPos, Point2 oldPos) {}

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};

}

#endif