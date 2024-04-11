#include "rg.hpp"

namespace RENI::rg {
	void Line2D::accept(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}

	void Rect2D::accept(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}
}