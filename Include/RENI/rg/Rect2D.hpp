#ifndef RENI_RG_RECT_2D_HEADER
#define RENI_RG_RECT_2D_HEADER

#include "../Point2D.hpp"
#include "RenderNode.hpp"

namespace RENI::rg {

	/**
	 * @brief Describes a rectangle in 2D space.
	*/
	class Rect2D : public RenderNode {
	public:

		Rect2D() = default;
		~Rect2D() override = default;


		void acceptVisitor(NodeVisitor& visitor) const override;


		Point2D topLeft;
		Point2D bottomRight;

	};

}

#endif