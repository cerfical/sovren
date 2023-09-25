#ifndef RENI_WIN_WND_CLASS_HEADER
#define RENI_WIN_WND_CLASS_HEADER

#include <Windows.h>

#include <string>
#include <memory>

namespace RENI {
	/**
	 * @brief Simple RAII wrapper for managing window classes.
	 */
	class WinWndClass {
	public:
		/** @{ */
		WinWndClass(const std::string& name, WNDPROC wndProc);
		/** @} */

		/** @{ */
		const std::string& name() const noexcept {
			return m_name;
		}
		ATOM atom() const noexcept {
			return m_atom.get();
		}
		/** @} */

	private:
		class AtomDeleter {
		public:
			using pointer = ATOM;
			void operator()(pointer atom);
		};

		std::unique_ptr<ATOM, AtomDeleter> m_atom;
		std::string m_name;
	};
}

#endif