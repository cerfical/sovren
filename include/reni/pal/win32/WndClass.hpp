#pragma once

#include "../../util/types.hpp"
#include "util.hpp"

#include <Windows.h>

#include <string>
#include <memory>

namespace reni::pal::win32 {
	
	class WndClass : private NonCopyable {
	public:
		
		WndClass() noexcept = default;

		WndClass(const std::string& name, WNDPROC wndProc);


		const std::string& name() const noexcept {
			return m_name;
		}
		
		ATOM atom() const noexcept {
			return m_atom.get().value;
		}


	private:
		struct Atom {
			
			Atom() noexcept = default;

			Atom(std::nullptr_t) noexcept
				: Atom(static_cast<ATOM>(0)) {}

			Atom(ATOM value) noexcept
				: value(value) {}

			explicit operator bool() const noexcept {
				return value != 0;
			}

			ATOM value = {};
		};

		class AtomDeleter {
		public:
			using pointer = Atom;
			
			void operator()(pointer atom) const {
				win32Check(UnregisterClass(MAKEINTATOM(atom.value), nullptr));
			}
		};

		std::unique_ptr<Atom, AtomDeleter> m_atom;
		std::string m_name;
	};

}