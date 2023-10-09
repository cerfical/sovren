#include "WndClass.hpp"
#include "Win32Utils.hpp"

namespace RENI {
	void WndClass::AtomDeleter::operator()(pointer atom) {
		win32Check(UnregisterClass(
			MAKEINTATOM(atom), NULL
		));
	}

	WndClass::WndClass(const std::string& name, WNDPROC wndProc)
		: m_name(name) {
		const auto tcName = mbToTc(name);
		WNDCLASSEX wndClass = {
			.cbSize = sizeof(wndClass),
			.style = 0,
			.lpfnWndProc = wndProc,
			.cbClsExtra = 0,
			.cbWndExtra = 0,
			.hInstance = NULL,
			.hIcon = NULL,
			.hCursor = NULL,
			.hbrBackground = NULL,
			.lpszMenuName = NULL,
			.lpszClassName = tcName.c_str(),
			.hIconSm = NULL
		};

		m_atom.reset(win32Check(RegisterClassEx(
			&wndClass
		)));
	}
}
