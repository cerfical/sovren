#include <RENI/RENI.hpp>
#include <iostream>

using namespace RENI;

class MainWindow : public Window {
private:
	void OnButtonPress() override {
		const auto& mouse = GetMouse();
		if(mouse.GetActiveButton() == MouseButtons::Left) {
			SetMouseCapture();
		}
	}
	void OnButtonRelease() override {
		const auto& mouse = GetMouse();
		if(mouse.GetActiveButton() == MouseButtons::Left) {
			ReleaseMouseCapture();
		}
	}

	void OnMouseMove() override {
		const auto& mouse = GetMouse();
		if(mouse.IsButtonPressed(MouseButtons::Left)) {
			const auto ctx = GetCanvas().BeginDraw();
			ctx->Clear({ 0, 0, 0 });
			ctx->SetDrawColor({ 64, 64, 255 });
			ctx->DrawLine({ .start = mouse.GetOldCursorPos(), .end = mouse.GetCursorPos() });
		}
	}

	void OnDraw() override {
		const auto ctx = GetCanvas().BeginDraw();
		ctx->Clear({ 0, 0, 0 });
	}

	void OnKeyPress() override {
		const auto& keys = GetKeys();
		std::cout << keys.GetActiveKey() << " pressed" << std::endl;
	}
};

int main() {
	try {
		MainWindow window;
		window.SetTitle("The Window");
		window.SetVisible(true);

		return GuiApp().Exec();
	} catch(...) { }
}