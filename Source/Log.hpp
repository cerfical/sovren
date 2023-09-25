#ifndef RENI_LOG_HEADER
#define RENI_LOG_HEADER

#include <spdlog/spdlog.h>	
#include <utility>

namespace RENI {
	/**
	 * @brief Provides basic logging capabilities.
	 */
	class Log {
	public:
		/** @{ */
		/** @brief Compose an informational message from the format string, then print it to the Log. */
		template <typename... Args>
		static void info(fmt::format_string<Args...> fmt, Args&&... args) noexcept {
			try {
				spdlog::info(fmt, std::forward<Args>(args)...);
			} catch(...) {
				// catch and discard all exceptions, as logging failure is of little interest
			}
		}
		/** @brief Print an informational message to the Log. */
		template <typename T>
		static void info(T&& msg) noexcept {
			info("{}", std::forward<T>(msg));
		}
		/** @} */

		/** @{ */
		/** @brief Compose an error message from the format string, then print it to the Log. */
		template <typename... Args>
		static void error(fmt::format_string<Args...> fmt, Args&&... args) noexcept {
			try {
				spdlog::error(fmt, std::forward<Args>(args)...);
			} catch(...) {
				// catch and discard all exceptions, as logging failure is of little interest
			}
		}
		/** @brief Print an error message to the Log. */
		template <typename T>
		static void error(T&& msg) noexcept {
			error("{}", std::forward<T>(msg));
		}
		/** @} */

	private:
		Log() = delete;
		~Log() = delete;
	};
}

#endif