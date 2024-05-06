#pragma once

#include "../util/types.hpp"
#include "RenderNode.hpp"

namespace reni::rg {

	/**
	 * @brief Describes a rectangle in 2D space.
	*/
	class Rect2D : public RenderNode {
	public:

		Rect2D() = default;
		~Rect2D() override = default;


		void accept(NodeVisitor& visitor) const override;


		Point2 topLeft;
		Size2 size;

	};

}