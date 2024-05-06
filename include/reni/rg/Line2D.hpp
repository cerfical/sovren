#pragma once

#include "../util/types.hpp"
#include "RenderNode.hpp"

namespace reni::rg {

	/**
	 * @brief Describes a straight line in 2D space.
	*/
	class Line2D : public RenderNode {
	public:

		Line2D() = default;
		~Line2D() override = default;


		void accept(NodeVisitor& visitor) const override;


		Point2 start;
		Point2 end;

	};

}