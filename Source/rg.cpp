#include "rg.hpp"

namespace RENI::rg {
	void Line2D::acceptVisitor(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}

	void Rect2D::acceptVisitor(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}
}