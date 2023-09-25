#include <RENI/RENI.hpp>
#include <iostream>

using namespace RENI;

class MainWindow : public Window {
private:
	void OnButtonPress() override {
		if(mouse.GetActiveButton() == MouseButtons::Left) {
			CaptureMouse();
		}
	}
	void OnButtonRelease() override {
		if(mouse.GetActiveButton() == MouseButtons::Left) {
			ReleaseMouse();
		}
	}

	void OnMouseMove() override {
		if(mouse.IsButtonPressed(MouseButtons::Left)) {
			const auto ctx = GetCanvas()->BeginDraw();
			ctx->Clear({ 0, 0, 0 });
			ctx->SetDrawColor({ 64, 64, 255 });
			ctx->DrawLine({ .start = mouse.GetOldCursorPos(), .end = mouse.GetCursorPos() });
		}
	}

	void OnDraw() override {
		const auto ctx = GetCanvas()->BeginDraw();
		ctx->Clear({ 0, 0, 0 });
	}

	void OnKeyPress() override {
		std::cout << keys.GetActiveKey() << " pressed" << std::endl;
	}
};

int main() {
	try {
		MainWindow window;
		window.SetTitle("The Window");
		window.SetVisible(true);

		return GuiApp().Exec();
	} catch(const std::exception& err) {
		std::cout << err.what() << std::endl;
	}
}