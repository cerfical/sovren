#include <RENI/RENI.hpp>
#include <iostream>

using namespace RENI;

int main() {
	auto window = Window();
	auto render = Render(window);

	window.SetTitle("Simple Window");
	window.SetVisible(true);

	auto eventLoop = EventLoop();
	while(eventLoop.HandleInput()) {
		auto ctx = render.StartRender();
		ctx.Clear({ 255, 0, 0 });
		
		const auto margin = 10;

		const auto left = window.IsKeyPressed(Keys::RightArrow) ? 0 : window.GetWidth() / 2 - margin;
		const auto right = window.IsKeyPressed(Keys::LeftArrow) ? 0 : window.GetWidth() / 2 - margin;

		const auto top = window.IsKeyPressed(Keys::DownArrow) ? 0 : window.GetHeight() / 2 - margin;
		const auto bottom = window.IsKeyPressed(Keys::UpArrow) ? 0 : window.GetHeight() / 2 - margin;

		ctx.DrawRect(Rect2D(
			Point2D(
				window.GetWidth() / 2 - left,
				window.GetHeight() / 2 - top
			),
			Size2D(
				left + right,
				top + bottom
			)
		));
	}
}