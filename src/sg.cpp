#include "sg/Camera3D.hpp"
#include "sg/Line2D.hpp"
#include "sg/NodeVisitor.hpp"
#include "sg/Rect2D.hpp"
#include "sg/Transform2D.hpp"
#include "sg/Transform3D.hpp"
#include "sg/Triangle3D.hpp"

namespace sovren {
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

    void Camera3D::accept(NodeVisitor& visitor) const {
        visitor.visit(*this);
    }
}
