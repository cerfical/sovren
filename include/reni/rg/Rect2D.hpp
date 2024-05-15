#pragma once

#include "../math/types.hpp"
#include "RenderNode.hpp"

namespace reni::rg {

	/**
	 * @brief Describes a rectangle in 2D space.
	*/
	class Rect2D : public RenderNode {
	public:

		/**
		 * @brief Construct a new rectangle from the top left and bottom right points.
		*/
		Rect2D(Vec2 topLeft, Vec2 bottomRight) noexcept
			: m_topLeft(topLeft), m_bottomRight(bottomRight) {}


		void accept(NodeVisitor& visitor) const override;


		/**
		 * @brief The point defining the top left corner of the rectangle.
		*/
		Vec2 topLeft() const noexcept {
			return m_topLeft;
		}


		/**
		 * @brief The point defining the bottom right corner of the rectangle.
		*/
		Vec2 bottomRight() const noexcept {
			return m_bottomRight;
		}


	private:
		Vec2 m_topLeft;
		Vec2 m_bottomRight;
	};

}