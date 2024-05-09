#pragma once

#include "../util/types.hpp"
#include "RenderNode.hpp"

namespace reni::rg {

	/**
	 * @brief Describes a straight line in 2D space.
	*/
	class Line2D : public RenderNode {
	public:

		Line2D(const Vec2& start, const Vec2& end) noexcept
			: m_start(start), m_end(end) {}


		void accept(NodeVisitor& visitor) const override;


		const Vec2& start() const noexcept {
			return m_start;
		}

		const Vec2& end() const noexcept {
			return m_end;
		}


	private:
		Vec2 m_start;
		Vec2 m_end;
	};

}