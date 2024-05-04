#ifndef RENI_RHI_DX_UTIL_HEADER
#define RENI_RHI_DX_UTIL_HEADER

#include <system_error>

#include <atlbase.h>
#include <d2d1.h>

namespace reni::rhi::dx {

	template <typename I>
		requires std::derived_from<I, IUnknown>
	using ComPtr = ATL::CComPtr<I>;


	inline HRESULT safeApiCall(HRESULT hr) {
		if(FAILED(hr)) {
			throw std::system_error(hr, std::system_category());
		}
		return hr;
	}

}

#endif