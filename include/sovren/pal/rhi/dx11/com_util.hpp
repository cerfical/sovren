#pragma once

#include <atlbase.h>

#include <concepts>
#include <system_error>

namespace sovren {

    template <std::derived_from<IUnknown> I>
    using ComPtr = ATL::CComPtr<I>;


    inline void comCheck(HRESULT hr) {
        if(FAILED(hr)) {
            throw std::system_error(hr, std::system_category());
        }
    }

}
