#include "WinUtils.hpp"
#include <gsl/util>

namespace RENI {
	std::wstring mbToWc(std::string_view str) {
		if(!str.empty()) {
			auto wcCount = safeWin32ApiCall(MultiByteToWideChar,
				CP_UTF8,
				0,
				str.data(),
				gsl::narrow_cast<int>(str.size()),
				nullptr,
				0
			); // calculate the number of wide chars needed to store the resulting string

			std::wstring wcStr(wcCount, L'\0');
			safeWin32ApiCall(MultiByteToWideChar,
				CP_UTF8,
				0,
				str.data(),
				gsl::narrow_cast<int>(str.size()),
				wcStr.data(),
				wcCount
			);
			return wcStr;
		}
		return L"";
	}

	std::string wcToMb(std::wstring_view str) {
		if(!str.empty()) {
			auto mbCount = safeWin32ApiCall(WideCharToMultiByte,
				CP_UTF8,
				0,
				str.data(),
				gsl::narrow_cast<int>(str.size()),
				nullptr,
				0,
				nullptr,
				nullptr
			); // calculate the number of bytes needed to store the resulting string

			std::string mbStr(mbCount, '\0');
			safeWin32ApiCall(WideCharToMultiByte,
				CP_UTF8,
				0,
				str.data(),
				gsl::narrow_cast<int>(str.size()),
				mbStr.data(),
				mbCount,
				nullptr,
				nullptr
			);
			return mbStr;
		}
		return "";
	}
}