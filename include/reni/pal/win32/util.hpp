#pragma once

#include <string_view>
#include <system_error>
#include <string>

#include <Windows.h>

namespace reni::pal::win32 {

	using TCharStringView = std::basic_string_view<TCHAR>;
	using TCharString = std::basic_string<TCHAR>;


	[[noreturn]]
	inline void raiseError(int code) {
		throw std::system_error(code, std::system_category());
	}
	

	[[noreturn]]
	inline void raiseError() {
		raiseError(GetLastError());
	}


	template <typename T>
	T safeApiCall(T res) {
		if(!res) {
			if(const auto err = GetLastError()) {
				raiseError(err);
			}
		}
		return res;
	}


	inline std::wstring mbToWc(std::string_view str) {
		if(!str.empty()) {
			auto wcCount = safeApiCall(MultiByteToWideChar(
				CP_UTF8,
				0,
				str.data(),
				static_cast<int>(str.size()),
				nullptr,
				0
			)); // calculate the number of wide chars needed to store the resulting string

			std::wstring wcStr(wcCount, L'\0');
			safeApiCall(MultiByteToWideChar(
				CP_UTF8,
				0,
				str.data(),
				static_cast<int>(str.size()),
				wcStr.data(),
				wcCount
			));
			return wcStr;
		}
		return L"";
	}


	inline std::string wcToMb(std::wstring_view str) {
		if(!str.empty()) {
			auto mbCount = safeApiCall(WideCharToMultiByte(
				CP_UTF8,
				0,
				str.data(),
				static_cast<int>(str.size()),
				nullptr,
				0,
				nullptr,
				nullptr
			)); // calculate the number of bytes needed to store the resulting string

			std::string mbStr(mbCount, '\0');
			safeApiCall(WideCharToMultiByte(
				CP_UTF8,
				0,
				str.data(),
				static_cast<int>(str.size()),
				mbStr.data(),
				mbCount,
				nullptr,
				nullptr
			));
			return mbStr;
		}
		return "";
	}


	inline std::string tcToMb(TCharStringView str) {
#ifdef UNICODE
		return wcToMb(str);
#else
		return std::string(str);
#endif
	}


	inline TCharString mbToTc(std::string_view str) {
#ifdef UNICODE
		return mbToWc(str);
#else
		return std::string(str);
#endif
	}

}