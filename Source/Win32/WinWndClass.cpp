#include "WinWndClass.hpp"

#include "WinUtils.hpp"
#include "Log.hpp"

namespace RENI {
	void WinWndClass::AtomDeleter::operator()(pointer atom) {
		safeWin32ApiCall(UnregisterClass,
			MAKEINTATOM(atom), nullptr
		);
		Log::info("Window class *:{0} unregistered", atom);
	}

	WinWndClass::WinWndClass(const std::string& name, WNDPROC wndProc)
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

		const auto atom = safeWin32ApiCall(RegisterClassEx,
			&wndClass
		);
		m_atom.reset(atom);

		Log::info("Window class {0}:{1} registered", name, atom);
	}
}