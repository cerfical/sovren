#include <sovren/sovren.hpp>

using namespace sovren;

auto main() noexcept -> int {
    const auto wnd = pal::Window::create();
    wnd->setTitle("window");
    wnd->setVisible(true);

    const auto events = pal::EventPump::create();
    while(true) {
        events->pumpEvents();
    }
}
