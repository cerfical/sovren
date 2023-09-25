#include <RENI/RENI.hpp>

int main() {
	RENI::Window window("Example Window", { 1024, 1024 });
	
	window.NotifyWhen(RENI::WindowEvent::Draw, [&]() {
		const auto ctx = window.GetCanvas().BeginDraw();
		ctx->Clear(RENI::Color::FromRgb(0, 255, 0));
		ctx->DrawRect({
			.topLeft = { 10, 10 }, .extent = { 250, 250 }
		}, RENI::Color::FromRgb(255, 0, 0));
	});
	window.SetVisible(true);

	return RENI::Ui::EnterUiLoop();
}