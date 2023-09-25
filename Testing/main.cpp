#include <RENI/RENI.hpp>
#include <iostream>

class DrawingView : private RENI::EventObserver {
public:
	DrawingView() {
		window.RegisterObserver(*this);
		window.SetTitle("Drawing Window");
		window.SetVisible(true);
	}
	~DrawingView() {
		window.UnregisterObserver(*this);
	}

	void StartDrawing() {
		RENI::Ui::EnterEventLoop();
	}

private:
	void OnMouseMove() override {
		const auto& mouse = window.GetMouseState();
		if(mouse.IsButtonPressed(RENI::MouseButtons::Left)) {
			const auto ctx = window.GetCanvas().BeginDraw();
			ctx->Clear({ 0, 0, 0 });
			ctx->SetDrawColor({ 64, 64, 255 });
			ctx->DrawLine({ .start = lineStart, .end = mouse.GetCursorPos() });
		}
		lineStart = mouse.GetCursorPos();
	}
	void OnWindowDraw() override {
		const auto ctx = window.GetCanvas().BeginDraw();
		ctx->Clear({ 0, 0, 0 });
	}

	RENI::Point2D lineStart;
	RENI::Window window;
};

int main() {
	try {
		auto drawView = DrawingView();
		drawView.StartDrawing();
	} catch(...) { }
}