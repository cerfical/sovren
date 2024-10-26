#pragma once

#include "../pal/Window.hpp"

#include "../Color.hpp"
#include "../Size2.hpp"

#include "SceneGraph.hpp"

#include <memory>

namespace sovren {

    class Renderer {
    public:

        explicit Renderer(Window& window);

        ~Renderer();


        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;

        Renderer(Renderer&&) noexcept;
        Renderer& operator=(Renderer&&) noexcept;


        void renderScene(const SceneGraph& scene);

        void setRenderSize(Size2 s);

        void setClearColor(Color clearCol);


        Color clearColor() const;


    private:
        struct Impl;
        std::unique_ptr<Impl> impl_;
    };

}
