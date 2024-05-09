#pragma once

#include "../util/types.hpp"

#include "Keys.hpp"
#include "MouseButtons.hpp"
#include "RenderGraph.hpp"

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

		void setFill(const Color& fill);

		const Color& fill() const;


		void show();


	protected:
		virtual void onResize(Size2 s);
		
		virtual void onShow();
		virtual void onUpdate();
		virtual void onHide();

		virtual void onKeyDown(Keys key);
		virtual void onKeyUp(Keys key);

		virtual void onButtonDown(MouseButtons but);
		virtual void onButtonUp(MouseButtons but);
		virtual void onMouseMove(Point2 p);

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};

}