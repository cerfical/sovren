#ifndef RENI_WIN_UTILS_HEADER
#define RENI_WIN_UTILS_HEADER

#include "Utils.hpp"

#include <string>
#include <string_view>
#include <system_error>
#include <concepts>

#include <atlbase.h>
#include <Windows.h>

namespace RENI {
	using TStringView = std::basic_string_view<TCHAR>;
	using TString = std::basic_string<TCHAR>;

	template <typename T>
	using ComPtr = ATL::CComPtr<T>;



	[[noreturn]] inline void raiseWin32Error(int errCode) {
		throw std::system_error(
			errCode, std::system_category()
		);
	}

	[[noreturn]] inline void raiseWin32Error() {
		raiseWin32Error(GetLastError());
	}



	template <typename C, typename... Args>
		requires std::invocable<C, Args...>
	auto safeWin32ApiCall(C&& func, Args&&... args) {
		struct ApiCall {
			ApiCall() noexcept {
				SetLastError(ERROR_SUCCESS);
			}
			~ApiCall() noexcept(false) {
				if(const auto err = GetLastError(); err != ERROR_SUCCESS) {
					raiseWin32Error(err);
				}
			}

			auto operator()(C&& func, Args&&... args) noexcept {
				return std::invoke(std::forward<C>(func), std::forward<Args>(args)...);
			}
		};

		return ApiCall()(std::forward<C>(func), std::forward<Args>(args)...);
	}



	template <typename C, typename... Args>
		requires std::same_as<std::invoke_result_t<C, Args...>, HRESULT>
			&& std::invocable<C, Args...>
	void safeComApiCall(C&& func, Args&&... args) {
		const auto result = std::invoke(std::forward<C>(func), std::forward<Args>(args)...);
		if(FAILED(result)) {
			raiseWin32Error(result);
		}
	}



	std::wstring mbToWc(std::string_view str);
	std::string wcToMb(std::wstring_view str);



	inline std::string tcToMb(TStringView str) {
#ifdef UNICODE
		return wcToMb(str);
#else
		return std::string(str);
#endif
	}

	inline TString mbToTc(std::string_view str) {
#ifdef UNICODE
		return mbToWc(str);
#else
		return std::string(str);
#endif
	}
}

#endif