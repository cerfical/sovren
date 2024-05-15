#pragma once

#include "../math/types.hpp"
#include "RenderNode.hpp"

namespace reni::rg {

	/**
	 * @brief Describes a straight line in 2D space.
	*/
	class Line2D : public RenderNode {
	public:

		/**
		 * @brief Construct a new line from the start and end points.
		*/
		Line2D(Vec2 start, Vec2 end) noexcept
			: m_start(start), m_end(end) {}


		void accept(NodeVisitor& visitor) const override;


		/**
		 * @brief Starting point of the line.
		*/
		Vec2 start() const noexcept {
			return m_start;
		}

		
		/**
		 * @brief Ending point of the line.
		*/
		Vec2 end() const noexcept {
			return m_end;
		}


	private:
		Vec2 m_start;
		Vec2 m_end;
	};

}