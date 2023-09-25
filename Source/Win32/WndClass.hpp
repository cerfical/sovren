#ifndef RENI_WND_CLASS_HEADER
#define RENI_WND_CLASS_HEADER

#include <Windows.h>

#include <string>
#include <memory>

namespace RENI::Win32 {
	/**
	 * @brief Simple RAII wrapper for managing window classes.
	 */
	class WndClass {
	public:
		/** @{ */
		/** @brief Construct a new WndClass with the specified name. */
		explicit WndClass(const std::string& name);
		
		/** @brief Construct a new WndClass with the specified name. */
		explicit WndClass(std::string&& name);

		/** @brief Destroy the WndClass. */
		~WndClass() = default;
		/** @} */

		/** @{ */
		WndClass(const WndClass&) = delete;
		WndClass& operator=(const WndClass&) = delete;
		/** @} */
		
		/** @{ */
		WndClass(WndClass&&) = delete;
		WndClass& operator=(WndClass&&) = delete;
		/** @} */

		/** @{ */
		/** @brief Get the name of the WndClass. */
		const std::string& GetName() const noexcept {
			return name;
		}

		/** @brief Get the atom of the WndClass. */
		ATOM GetAtom() const noexcept {
			return atom.get();
		}
		/** @} */

	private:
		/**
		 * @brief @c std::unique_ptr<> deleter that properly unregisters the window class.
		 */
		class WndClassDeleter {
		public:
			using pointer = ATOM;

			void operator()(pointer atom);
		};

		std::unique_ptr<ATOM, WndClassDeleter> atom;
		std::string name;
	};
}

#endif