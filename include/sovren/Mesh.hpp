#pragma once

#include <sovren/math/Vec3.hpp>

#include <initializer_list>
#include <span>
#include <vector>

namespace sovren {

    class Mesh {
    public:
        Mesh() = default;

        Mesh(std::vector<Vec3> vertices)
            : vertices_(std::move(vertices)) {}

        Mesh(std::initializer_list<Vec3> vertices)
            : Mesh(std::vector<Vec3>(vertices)) {}

        Mesh(const Mesh&) = default;
        auto operator=(const Mesh&) -> Mesh& = default;

        Mesh(Mesh&&) = default;
        auto operator=(Mesh&&) -> Mesh& = default;

        ~Mesh() = default;

        [[nodiscard]]
        auto vertices() const noexcept -> std::span<const Vec3> {
            return vertices_;
        }

        [[nodiscard]]
        auto size() const noexcept -> std::size_t {
            return vertices_.size();
        }

    private:
        std::vector<Vec3> vertices_;
    };

}
