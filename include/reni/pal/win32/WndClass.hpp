#pragma once

#include "../../util/types.hpp"
#include "util.hpp"

#include <Windows.h>

#include <string>
#include <memory>

namespace reni::pal::win32 {
	
	class WndClass : private NonCopyable {
	public:
		
		WndClass() = default;

		WndClass(const std::string& name, WNDPROC wndProc)
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
			m_atom.reset(safeApiCall(RegisterClassEx(&wndClass)));
		}


		const std::string& name() const {
			return m_name;
		}
		
		ATOM atom() const {
			return m_atom.get().value;
		}


	private:
		struct Atom {
			
			Atom() = default;

			Atom(std::nullptr_t)
				: Atom(static_cast<ATOM>(0)) {}

			Atom(ATOM value)
				: value(value) {}


			explicit operator bool() const {
				return value != 0;
			}


			ATOM value = {};
		};

		class AtomDeleter {
		public:
			using pointer = Atom;
			
			void operator()(pointer atom) {
				safeApiCall(UnregisterClass(MAKEINTATOM(atom.value), nullptr));
			}
		};

		std::unique_ptr<Atom, AtomDeleter> m_atom;
		std::string m_name;
	};

}