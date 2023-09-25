#include <RENI/RENI.hpp>
#include <iostream>

class DrawingView : private RENI::WindowObserver, private RENI::MouseObserver {
public:
	DrawingView() {
		window.AddObserver(*this);
		window.GetMouseState().AddObserver(*this);

		window.SetTitle("Drawing Window");
		window.SetVisible(true);
	}
	~DrawingView() {
		window.GetMouseState().RemoveObserver(*this);
		window.RemoveObserver(*this);
	}

	void StartDrawing() {
		RENI::Ui::EnterEventLoop();
	}

private:
	void OnMouseButtonPress(RENI::MouseButtons pressedButton) override {
		const auto& mouse = window.GetMouseState();
		if(pressedButton == RENI::MouseButtons::Left) {
			lineStart = mouse.GetCursorPos();
			window.SetMouseCapture();
		}
	}
	void OnMouseButtonRelease(RENI::MouseButtons releasedButton) override {
		if(releasedButton == RENI::MouseButtons::Left) {
			window.ReleaseMouseCapture();
		}
	}

	void OnMouseMove(const RENI::Displace2D& mouseDisplace) override {
		const auto& mouse = window.GetMouseState();
		if(mouse.IsButtonPressed(RENI::MouseButtons::Left)) {
			const auto ctx = window.GetCanvas().BeginDraw();
			ctx->Clear({ 0, 0, 0 });
			ctx->SetDrawColor({ 64, 64, 255 });
			ctx->DrawLine({ .start = lineStart, .end = mouse.GetCursorPos() });
			lineStart = mouse.GetCursorPos();
		}
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