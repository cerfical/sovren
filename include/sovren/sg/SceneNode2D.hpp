#pragma once

#include <sovren/sg/RenderNode.hpp>

#include <sovren/Transform2D.hpp>

namespace sovren::sg {

    class SceneNode2D : public RenderNode {
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
