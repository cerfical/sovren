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
		/** @brief Register a new window class with the specified name and window procedure. */
		explicit WinWndClass(const std::string& name, WNDPROC wndProc);

		/** @brief Unregister the class. */
		~WinWndClass() = default;
		/** @} */

		/** @{ */
		WinWndClass(WinWndClass&&) = default;
		WinWndClass& operator=(WinWndClass&&) = default;
		/** @} */

		/** @{ */
		WinWndClass(const WinWndClass&) = delete;
		WinWndClass& operator=(const WinWndClass&) = delete;
		/** @} */

		/** @{ */
		/** @brief Get a string that uniquely identifies the class. */
		const std::string& GetName() const noexcept {
			return m_name;
		}

		/** @brief Get an integer uniquely identifying the class. */
		ATOM GetAtom() const noexcept {
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