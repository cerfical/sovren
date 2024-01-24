#ifndef RENI_RG_LINE_2D_HEADER
#define RENI_RG_LINE_2D_HEADER

#include "../Point2D.hpp"
#include "RenderNode.hpp"

namespace RENI::rg {

	/**
	 * @brief Describes a straight line in 2D space.
	*/
	class Line2D : public RenderNode {
	public:

		Line2D(Point2D start, Point2D end)
			: start(start), end(end)
		{ }

		~Line2D() override = default;



		void acceptVisitor(NodeVisitor& visitor) const override;



		/**
		 * @brief The start point of the line.
		*/
		Point2D start;



		/**
		 * @brief The end point of the line.
		*/
		Point2D end;

	};

}

#endif