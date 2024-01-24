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

		Line2D() = default;
		~Line2D() override = default;


		void acceptVisitor(NodeVisitor& visitor) const override;


		Point2D start;
		Point2D end;

	};

}

#endif