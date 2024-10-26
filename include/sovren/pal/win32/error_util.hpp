#pragma once

#include <system_error>

#include <Windows.h>

namespace sovren::win32 {

    [[noreturn]]
    inline void raiseError(int code) {
        throw std::system_error(code, std::system_category());
    }


    [[noreturn]]
    inline void raiseError() {
        raiseError(static_cast<int>(GetLastError()));
    }


    template <typename T>
    auto win32Check(T res) -> T {
        if(!res) {
            if(const auto err = GetLastError()) {
                raiseError(static_cast<int>(err));
            }
        }
        return res;
    }

}
