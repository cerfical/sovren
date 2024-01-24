#ifndef RENI_RG_RECT_2D_HEADER
#define RENI_RG_RECT_2D_HEADER

#include "../Utils.hpp"
#include "RenderNode.hpp"

namespace RENI::rg {

	/**
	 * @brief Describes a rectangle in 2D space.
	*/
	class Rect2D : public RenderNode {
	public:

		Rect2D(Point2D topLeft, Point2D bottomRight)
			: topLeft(topLeft), bottomRight(bottomRight)
		{ }

		~Rect2D() override = default;



		void acceptVisitor(NodeVisitor& visitor) const override;



		/**
		 * @brief The top left point of the rectangle.
		*/
		Point2D topLeft;



		/**
		 * @brief The bottom right point of the rectangle.
		*/
		Point2D bottomRight;

	};

}

#endif