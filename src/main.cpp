#include <sovren/RenderContext.hpp>
#include <sovren/sovren.hpp>

using namespace sovren;


auto main() noexcept -> int {
    const auto wnd = Window::create();
    wnd->setTitle("window");
    wnd->setVisible(true);

    const auto events = EventPump::create();
    auto ctx = RenderContext(wnd->handle());

    while(true) {
        events->pumpEvents();

        ctx.startFrame();
        ctx.endFrame();
    }
}
