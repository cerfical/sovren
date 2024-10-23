#pragma once

#include <string>
#include <string_view>
#include <system_error>

#include <Windows.h>

namespace sovren::pal::win32 {

    using tstring_view = std::basic_string_view<TCHAR>;
    using tstring = std::basic_string<TCHAR>;


    [[noreturn]]
    inline void raiseError(int code) {
        throw std::system_error(code, std::system_category());
    }


    [[noreturn]]
    inline void raiseError() {
        raiseError(GetLastError());
    }


    template <typename T>
    T win32Check(T res) {
        if(!res) {
            if(const auto err = GetLastError()) {
                raiseError(err);
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
