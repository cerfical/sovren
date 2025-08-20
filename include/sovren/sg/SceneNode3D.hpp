#pragma once

#include <sovren/sg/SceneNode.hpp>

#include <sovren/RenderQueue3D.hpp>
#include <sovren/Transform3D.hpp>

#include <optional>

namespace sovren::sg {

    class SceneNode3D : public SceneNode {
    public:
        virtual void render(RenderQueue3D&) const {}

        [[nodiscard]]
        auto transform() const noexcept -> const Transform3D& {
            return transform_;
        }

        [[nodiscard]]
        auto transform() noexcept -> Transform3D& {
            resetWorldTransform();
            return transform_;
        }

        [[nodiscard]]
        auto worldTransform() const noexcept -> const Transform3D& {
            if(!worldTransform_) {
                if(const auto* node3D = findUp<SceneNode3D>()) {
                    worldTransform_ = node3D->worldTransform() * transform();
                } else {
                    worldTransform_ = transform();
                }
            }
            return *worldTransform_;
        }

    private:
        static void resetWorldTransforms(SceneNode& n) noexcept {
            for(const auto& child : n.children()) {
                if(const auto node3D = std::dynamic_pointer_cast<SceneNode3D>(child)) {
                    node3D->resetWorldTransform();
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

        mutable std::optional<Transform3D> worldTransform_;
        Transform3D transform_;
    };

}
