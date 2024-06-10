#pragma once

#include "../util/Color.hpp"
#include "../util/Size2.hpp"

#include <memory>

namespace reni {

    class Window;
    class RenderGraph;


    class Renderer {
    public:

        explicit Renderer(Window& window);

        ~Renderer();


        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;

        Renderer(Renderer&&) noexcept;
        Renderer& operator=(Renderer&&) noexcept;


        void renderScene(const RenderGraph& scene);

        void setRenderSize(Size2 s);

        void setClearColor(Color clearCol);


        Color clearColor() const;


    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };

}
