#pragma once

#include "SceneNode.hpp"
#include "Transform2D.hpp"

namespace sovren {

    class SceneNode2D : public SceneNode {
    public:

        [[nodiscard]]
        auto transform() const noexcept -> const Transform2D& {
            return transform_;
        }

        [[nodiscard]]
        auto transform() noexcept -> Transform2D& {
            return transform_;
        }

    private:
        Transform2D transform_;
    };

}
