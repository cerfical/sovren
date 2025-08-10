#pragma once

#include <sovren/RenderNode.hpp>
#include <sovren/Transform3D.hpp>

namespace sovren {

    class SceneNode3D : public RenderNode {
    public:
        [[nodiscard]]
        auto transform() const noexcept -> const Transform3D& {
            return transform_;
        }

        [[nodiscard]]
        auto transform() noexcept -> Transform3D& {
            return transform_;
        }

    private:
        Transform3D transform_;
    };

}
