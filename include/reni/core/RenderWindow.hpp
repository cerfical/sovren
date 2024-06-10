#pragma once

#include "Renderer.hpp"
#include "SceneGraph.hpp"
#include "Window.hpp"

namespace reni {

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


        float aspectRatio() const {
            const auto s = size();
            return s.width / s.height;
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
