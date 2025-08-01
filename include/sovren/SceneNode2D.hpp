#pragma once

#include "RenderNode.hpp"
#include "Transform2D.hpp"

namespace sovren {

    class SceneNode2D : public RenderNode {
    public:
        void render() {
            onRender();
        }

        [[nodiscard]]
        auto transform() const noexcept -> const Transform2D& {
            return transform_;
        }

        [[nodiscard]]
        auto transform() noexcept -> Transform2D& {
            return transform_;
        }

    protected:
        virtual void onRender() {}

    private:
        Transform2D transform_;
    };

}
