#ifndef RENI_WIN32_UTILS_HEADER
#define RENI_WIN32_UTILS_HEADER

#include <string_view>
#include <string>

#include <Windows.h>

namespace RENI {

	using tstring_view = std::basic_string_view<TCHAR>;
	using tstring = std::basic_string<TCHAR>;


	[[noreturn]]
	inline void win32Error(int code);
	
	[[noreturn]]
	inline void win32Error();


	template <typename T>
	T win32Check(T res) {
		if(!res) {
			if(const auto err = GetLastError()) {
				win32Error(err);
			}
		}
		return res;
	}


	std::wstring mbToWc(std::string_view str);
	std::string wcToMb(std::wstring_view str);


	inline std::string tcToMb(tstring_view str) {
#ifdef UNICODE
		return wcToMb(str);
#else
		return std::string(str);
#endif
	}

	inline tstring mbToTc(std::string_view str) {
#ifdef UNICODE
		return mbToWc(str);
#else
		return std::string(str);
#endif
	}

}

#endif