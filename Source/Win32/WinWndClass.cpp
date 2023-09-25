#include "WinWndClass.hpp"

#include "WinUtils.hpp"
#include "Log.hpp"

namespace RENI {
	void WinWndClass::AtomDeleter::operator()(pointer atom) {
		SafeWin32ApiCall(UnregisterClass,
			MAKEINTATOM(atom), nullptr
		);
		Log::Info("Window class *:{0:#x} unregistered", atom);
	}

	WinWndClass::WinWndClass(const std::string& name, WNDPROC wndProc)
		: m_name(name) {
		const auto tcName = MbToTc(name);
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

		const auto atom = SafeWin32ApiCall(RegisterClassEx,
			&wndClass
		);
		m_atom.reset(atom);

		Log::Info("Window class {0}:{1:#x} registered", name, atom);
	}
}