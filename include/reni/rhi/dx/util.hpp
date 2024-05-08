#pragma once

#include <system_error>
#include <atlbase.h>

namespace reni::rhi::dx {

	template <typename I>
		requires std::derived_from<I, IUnknown>
	using ComPtr = ATL::CComPtr<I>;


	inline void comCheck(HRESULT hr) {
		if(FAILED(hr)) {
			throw std::system_error(hr, std::system_category());
		}
	}

}