#pragma once

#include <sovren/sg/SceneNode.hpp>

#include <sovren/Transform2D.hpp>

#include <optional>

namespace sovren::sg {

    class SceneNode2D : public SceneNode {
    public:
        [[nodiscard]]
        auto transform() const noexcept -> const Transform2D& {
            return transform_;
        }

        [[nodiscard]]
        auto transform() noexcept -> Transform2D& {
            resetWorldTransform();
            return transform_;
        }

        [[nodiscard]]
        auto worldTransform() const noexcept -> const Transform2D& {
            if(!worldTransform_) {
                if(const auto* node2D = findUp<SceneNode2D>()) {
                    worldTransform_ = node2D->worldTransform() * transform();
                } else {
                    worldTransform_ = transform();
                }
            }
            return *worldTransform_;
        }

    private:
        static void resetWorldTransforms(SceneNode& n) noexcept {
            for(const auto& child : n.children()) {
                if(const auto node2D = std::dynamic_pointer_cast<SceneNode2D>(child)) {
                    node2D->resetWorldTransform();
                } else {
                    resetWorldTransforms(*child);
                }
            }
        }

        void resetWorldTransform() noexcept {
            if(!worldTransform_) {
                // if the parent transform is reset, the child transforms are also reset
                // so, there is nothing to do
                return;
            }
            worldTransform_.reset();

            // propagate the reset to the children
            resetWorldTransforms(*this);
        }

        mutable std::optional<Transform2D> worldTransform_;
        Transform2D transform_;
    };

}
