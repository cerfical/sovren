#pragma once

#include "math/Vec3.hpp"

#include <initializer_list>
#include <vector>

namespace sovren {

    class Mesh {
    public:

        explicit Mesh(std::initializer_list<Vec3> verts)
            : vertices_(verts) {}

        [[nodiscard]]
        auto vertices() const noexcept -> const std::vector<Vec3>& {
            return vertices_;
        }

    private:
        std::vector<Vec3> vertices_;
    };

}
