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
		/** @brief Print an informational message to the Log. */
		template <typename... Args>
		static void Info(fmt::format_string<Args...> fmt, Args&&... args) noexcept {
			try {
				spdlog::info(fmt, std::forward<Args>(args)...);
			} catch(...) {
				// just ignore all errors, as logging failure is not that important
			}
		}

		/** @brief Print an informational message to the Log. */
		template <typename T>
		static void Info(T&& msg) noexcept {
			Info("{}", std::forward<T>(msg));
		}

	private:
		Log() = delete;
		~Log() = delete;
	};
}

#endif