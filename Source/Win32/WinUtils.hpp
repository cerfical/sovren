#ifndef RENI_WIN_UTILS_HEADER
#define RENI_WIN_UTILS_HEADER

#include <string>
#include <string_view>
#include <system_error>
#include <concepts>

#include <Windows.h>

namespace RENI {
	using TStringView = std::basic_string_view<TCHAR>;
	using TString = std::basic_string<TCHAR>;



	/** @brief Raise a Win32-specific exception with the specified error code. */
	[[noreturn]] inline void RaiseWin32Error(int errCode) {
		throw std::system_error(
			errCode, std::system_category()
		);
	}

	/** @brief Raise a Win32-specific exception with the error code retrieved from GetLastError(). */
	[[noreturn]] inline void RaiseWin32Error() {
		RaiseWin32Error(GetLastError());
	}

	/** @brief Perform a safe Win32 API call. */
	template <typename C, typename... Args>
		requires std::invocable<C, Args...>
	auto SafeWin32ApiCall(C&& func, Args&&... args) {
		SetLastError(ERROR_SUCCESS);
		const auto result = std::invoke(std::forward<C>(func), std::forward<Args>(args)...);
		if(const auto errorCode = GetLastError(); errorCode != ERROR_SUCCESS) {
			RaiseWin32Error(errorCode);
		}
		return result;
	}



	/** @brief Perform a safe COM API call. */
	template <typename C, typename... Args>
		requires std::same_as<std::invoke_result_t<C, Args...>, HRESULT>
			&& std::invocable<C, Args...>
	void SafeComApiCall(C&& func, Args&&... args) {
		const auto result = std::invoke(std::forward<C>(func), std::forward<Args>(args)...);
		if(FAILED(result)) {
			RaiseWin32Error(result);
		}
	}



	/** @brief Convert a multibyte UTF-8 string to a wide character UTF-16 string. */
	std::wstring MbToWc(std::string_view str);

	/** @brief Convert a wide character UTF-16 string to a multibyte UTF-8 string. */
	std::string WcToMb(std::wstring_view str);



	/** @brief Convert a TCHAR string to a multibyte UTF-8 string. */
	inline std::string TcToMb(TStringView str) {
#ifdef UNICODE
		return WcToMb(str);
#else
		return std::string(str);
#endif
	}

	/** @brief Convert a multibyte UTF-8 string to a TCHAR string. */
	inline TString MbToTc(std::string_view str) {
#ifdef UNICODE
		return MbToWc(str);
#else
		return std::string(str);
#endif
	}
}

#endif