#pragma once

#include <sovren/math/Vec3.hpp>

#include <initializer_list>
#include <memory>
#include <span>
#include <vector>

namespace sovren {

    class Mesh {
    public:
        Mesh() noexcept = default;

        Mesh(std::vector<Vec3> vertices)
            : vertices_(std::make_shared<std::vector<Vec3>>(std::move(vertices))) {}

        Mesh(std::initializer_list<Vec3> vertices)
            : Mesh(std::vector<Vec3>(vertices)) {}

        Mesh(const Mesh&) noexcept = default;
        auto operator=(const Mesh&) noexcept -> Mesh& = default;

        Mesh(Mesh&&) noexcept = default;
        auto operator=(Mesh&&) noexcept -> Mesh& = default;

        ~Mesh() = default;

        [[nodiscard]]
        auto vertices() const noexcept -> std::span<Vec3> {
            if(vertices_) {
                return *vertices_;
            }
            return {};
        }

    private:
        std::shared_ptr<std::vector<Vec3>> vertices_;
    };

}
