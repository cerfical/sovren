#include "rg.hpp"

namespace reni::rg {
	void Line2D::accept(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}

	void Rect2D::accept(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}

	void Transform2D::accept(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}

	void Triangle3D::accept(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}

	void Transform3D::accept(NodeVisitor& visitor) const {
		visitor.visit(*this);
	}
}