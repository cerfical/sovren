#include "utils.hpp"

#include <system_error>
#include <gsl/util>

namespace RENI {
	void win32Error(int code) {
		throw std::system_error(
			code, std::system_category()
		);
	}

	void win32Error() {
		win32Error(GetLastError());
	}


	std::wstring mbToWc(std::string_view str) {
		if(!str.empty()) {
			auto wcCount = win32Check(MultiByteToWideChar(
				CP_UTF8,
				0,
				str.data(),
				gsl::narrow_cast<int>(str.size()),
				NULL,
				0
			)); // calculate the number of wide chars needed to store the resulting string

			std::wstring wcStr(wcCount, L'\0');
			win32Check(MultiByteToWideChar(
				CP_UTF8,
				0,
				str.data(),
				gsl::narrow_cast<int>(str.size()),
				wcStr.data(),
				wcCount
			));
			return wcStr;
		}
		return L"";
	}

	std::string wcToMb(std::wstring_view str) {
		if(!str.empty()) {
			auto mbCount = win32Check(WideCharToMultiByte(
				CP_UTF8,
				0,
				str.data(),
				gsl::narrow_cast<int>(str.size()),
				NULL,
				0,
				NULL,
				NULL
			)); // calculate the number of bytes needed to store the resulting string

			std::string mbStr(mbCount, '\0');
			win32Check(WideCharToMultiByte(
				CP_UTF8,
				0,
				str.data(),
				gsl::narrow_cast<int>(str.size()),
				mbStr.data(),
				mbCount,
				NULL,
				NULL
			));
			return mbStr;
		}
		return "";
	}
}