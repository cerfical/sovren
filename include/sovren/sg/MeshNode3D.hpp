#pragma once

#include "NodeVisitor.hpp"
#include "SceneNode3D.hpp"

#include <vector>

namespace sovren {

    class MeshNode3D : public SceneNode3D {
    public:

        explicit MeshNode3D(std::vector<Vec3> verts)
            : vertices_(std::move(verts)) {}


        void acceptVisitor(NodeVisitor& visitor) const override {
            visitor.visit(*this);
        }


        [[nodiscard]]
        auto vertices() const noexcept -> const std::vector<Vec3>& {
            return vertices_;
        }


    private:
        std::vector<Vec3> vertices_;
    };

}
