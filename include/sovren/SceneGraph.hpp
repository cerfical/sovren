#pragma once

#include "pal/EventPump.hpp"
#include "pal/Window.hpp"

#include "Input.hpp"
#include "Render.hpp"
#include "SceneNode.hpp"

namespace sovren {

    class SceneGraph : public SceneNode {
    public:
        SceneGraph()
            : window_(Window::create())
            , events_(EventPump::create())
            , input_(makeNode<Input>(window_))
            , render_(makeNode<Render>(window_)) {
            input_->addChild(render_);
        }

        void addNode(NodePtr node) {
            render_->addChild(node);
        }

        [[nodiscard]]
        auto nodes() const noexcept -> const NodeList& {
            return render_->children();
        }

        void present() {
            window_->setVisible(true);
            while(window_->isVisible()) {
                events_->pumpEvents();
                render_->update();
            }
        }

    private:
        std::shared_ptr<Window> window_;
        std::unique_ptr<EventPump> events_;

        NodePtr input_;
        NodePtr render_;
    };

}
