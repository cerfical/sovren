#pragma once

#include "Renderer.hpp"
#include "SceneGraph.hpp"
#include "Window.hpp"

namespace sovren {

    class RenderWindow : public Window {
    public:

        SceneGraph& scene() {
            return scene_;
        }


        void setFill(Color fill) {
            renderer_.setClearColor(fill);
        }

        Color fill() const {
            return renderer_.clearColor();
        }


    protected:
        void onResize() override {
            renderer_.setRenderSize(size());
        }

        void onUpdate() override {
            renderer_.renderScene(scene_);
        }

    private:
        Renderer renderer_ = Renderer(*this);
        SceneGraph scene_;
    };

}
