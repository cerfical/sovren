#pragma once

#include <sovren/sg/SceneNode.hpp>

#include <sovren/RenderQueue2D.hpp>
#include <sovren/Transform2D.hpp>

namespace sovren::sg {

    class SceneNode2D : public SceneNode {
    public:
        virtual void render(RenderQueue2D& queue) const {}

        [[nodiscard]]
        auto transform() const noexcept -> const Transform2D& {
            return transform_;
        }

        [[nodiscard]]
        auto transform() noexcept -> Transform2D& {
            return transform_;
        }

        [[nodiscard]]
        auto worldTransform() const noexcept -> Transform2D {
            if(const auto* node2D = findUp<SceneNode2D>()) {
                return node2D->worldTransform() * transform();
            }
            return transform();
        }

    private:
        Transform2D transform_;
    };

}
