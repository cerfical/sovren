#pragma once

#include "../util/types.hpp"
#include "RenderNode.hpp"

namespace reni::rg {

	/**
	 * @brief Describes a rectangle in 2D space.
	*/
	class Rect2D : public RenderNode {
	public:

		Rect2D(const Vec2& topLeft, const Vec2& botRight) noexcept
			: m_topLeft(topLeft), m_botRight(botRight) {}


		void accept(NodeVisitor& visitor) const override;


		const Vec2& topLeft() const noexcept {
			return m_topLeft;
		}

		const Vec2& botRight() const noexcept {
			return m_botRight;
		}


	private:
		Vec2 m_topLeft;
		Vec2 m_botRight;
	};

}