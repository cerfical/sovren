#pragma once

#include <system_error>

#include <Windows.h>

namespace sovren::win32 {

    [[noreturn]]
    inline void raiseError(DWORD code) {
        throw std::system_error(static_cast<int>(code), std::system_category());
    }


    [[noreturn]]
    inline void raiseError() {
        raiseError(GetLastError());
    }


    template <typename T>
    auto win32Check(T res) -> T {
        if(!res) {
            if(const auto err = GetLastError()) {
                raiseError(err);
            }
        }
        return res;
    }

}
