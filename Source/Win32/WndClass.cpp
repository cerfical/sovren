#include "WndClass.hpp"

#include "WinUtils.hpp"
#include "WndProc.hpp"

#include "Log.hpp"

namespace {
	using namespace RENI::Win32;
	using namespace RENI;

	ATOM RegisterWndClass(std::string_view name) {
		const auto tcName = MbToTc(name);
		WNDCLASSEX wndClass = {
			.cbSize = sizeof(wndClass),
			.style = 0,
			.lpfnWndProc = WndProc::Get(),
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
		Log::Info("Window class {0}:{1:#x} registered", name, atom);
		return atom;
	}
}

namespace RENI::Win32 {
	WndClass::WndClass(const std::string& name)
	 : name(name) {
		atom.reset(RegisterWndClass(name));
	}

	WndClass::WndClass(std::string&& name)
	 : name(std::move(name)) {
		atom.reset(RegisterWndClass(this->name));
	}

	void WndClass::WndClassDeleter::operator()(pointer atom) {
		SafeWin32ApiCall(UnregisterClass,
			MAKEINTATOM(atom), nullptr
		);
		Log::Info("Window class *:{0:#x} unregistered", atom);
	}
}