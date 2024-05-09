#include "pal/win32/WndClass.hpp"

namespace reni::pal::win32 {
	WndClass::WndClass(const std::string& name, WNDPROC wndProc)
		: m_name(name) {

		const auto tcName = mbToTc(name);
		WNDCLASSEX wndClass = {
			.cbSize = sizeof(wndClass),
			.style = 0,
			.lpfnWndProc = wndProc,
			.cbClsExtra = 0,
			.cbWndExtra = 0,
			.hInstance = nullptr,
			.hIcon = nullptr,
			.hCursor = nullptr,
			.hbrBackground = nullptr,
			.lpszMenuName = nullptr,
			.lpszClassName = tcName.c_str(),
			.hIconSm = nullptr
		};
		m_atom.reset(win32Check(RegisterClassEx(&wndClass)));
	}
}