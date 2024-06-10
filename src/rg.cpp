#include "rg/Camera3D.hpp"
#include "rg/Line2D.hpp"
#include "rg/NodeVisitor.hpp"
#include "rg/Rect2D.hpp"
#include "rg/Transform2D.hpp"
#include "rg/Transform3D.hpp"
#include "rg/Triangle3D.hpp"

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

    void Camera3D::accept(NodeVisitor& visitor) const {
        visitor.visit(*this);
    }
}
