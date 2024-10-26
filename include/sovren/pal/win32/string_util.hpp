#pragma once

#include "error_util.hpp"

#include <string>
#include <string_view>

#include <Windows.h>

namespace sovren::win32 {

    using TStringView = std::basic_string_view<TCHAR>;
    using TString = std::basic_string<TCHAR>;


    [[nodiscard]]
    inline auto mbToWc(std::string_view str) -> std::wstring {
        if(!str.empty()) {
            auto wcCount = win32Check(MultiByteToWideChar(
                CP_UTF8,
                0,
                str.data(),
                static_cast<int>(str.size()),
                nullptr,
                0
            )); // calculate the number of wide chars needed to store the resulting string

            std::wstring wcStr(wcCount, L'\0');
            win32Check(MultiByteToWideChar(
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


    [[nodiscard]]
    inline auto wcToMb(std::wstring_view str) -> std::string {
        if(!str.empty()) {
            auto mbCount = win32Check(WideCharToMultiByte(
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
            win32Check(WideCharToMultiByte(
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


    [[nodiscard]]
    inline auto tcToMb(TStringView str) -> std::string {
#ifdef UNICODE
        return wcToMb(str);
#else
        return std::string(str);
#endif
    }


    [[nodiscard]]
    inline auto mbToTc(std::string_view str) -> TString {
#ifdef UNICODE
        return mbToWc(str);
#else
        return std::string(str);
#endif
    }

}
