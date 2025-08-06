#pragma once

#include "RenderNode.hpp"
#include "Transform3D.hpp"

namespace sovren {

    class SceneNode3D : public RenderNode {
    public:
        void render() {
            onRender();
        }

        [[nodiscard]]
        auto transform() const noexcept -> const Transform3D& {
            return transform_;
        }

        [[nodiscard]]
        auto transform() noexcept -> Transform3D& {
            return transform_;
        }

    protected:
        virtual void onRender() {}

    private:
        Transform3D transform_;
    };

}
