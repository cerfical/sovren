#include "rhi/dx/DxSwapChain.hpp"
#include <dxgi1_2.h>

namespace sovren::rhi::dx {
    DxSwapChain::DxSwapChain(HWND window, ID3D11Device* d3dDevice, ID2D1DeviceContext* d2dContext)
        : m_d2dContext(d2dContext) {

        // retrieve the IDXGIFactory2 interface associated with the ID3D11Device
        ComPtr<IDXGIDevice> dxgiDevice;
        comCheck(d3dDevice->QueryInterface(IID_PPV_ARGS(&dxgiDevice)));

        ComPtr<IDXGIAdapter> dxgiAdapter;
        comCheck(dxgiDevice->GetAdapter(&dxgiAdapter));

        ComPtr<IDXGIFactory2> dxgiFactory;
        comCheck(dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory)));

        // initialize and create a swap chain
        DXGI_SWAP_CHAIN_DESC1 scDesc = {
            0,
            0, // make the buffer size be equal to the window size
            DXGI_FORMAT_R8G8B8A8_UNORM,
            FALSE, // no stereo mode
            1,
            0, // sample description
            DXGI_USAGE_RENDER_TARGET_OUTPUT,
            2, // buffer count
            DXGI_SCALING_NONE,
            DXGI_SWAP_EFFECT_FLIP_DISCARD,
            DXGI_ALPHA_MODE_UNSPECIFIED,
            0 // no flags
        };

        ComPtr<IDXGISwapChain1> swapChain;
        comCheck(dxgiFactory->CreateSwapChainForHwnd(
            d3dDevice,
            window, // output window
            &scDesc,
            nullptr, // no fullscreen description
            nullptr, // not restricted to some IDXGIOutput
            &swapChain
        ));
        comCheck(swapChain->GetDesc1(&scDesc));

        m_swapChain = swapChain;
        setBufferSize({ static_cast<float>(scDesc.Width),
                        static_cast<float>(scDesc.Height) });
    }


    void DxSwapChain::setBufferSize(Size2 newSize) {
        m_frontBuffer.reset();
        comCheck(m_swapChain->ResizeBuffers(
            0, // preserve the existing number of buffers
            static_cast<UINT>(newSize.width),
            static_cast<UINT>(newSize.height),
            DXGI_FORMAT_UNKNOWN, // no change to buffer format
            0                    // no flags
        ));

        ComPtr<IDXGISurface> frontBuffer;
        comCheck(m_swapChain->GetBuffer(0, IID_PPV_ARGS(&frontBuffer)));
        m_frontBuffer.emplace(frontBuffer, m_d2dContext);

        m_bufferSize = newSize;
    }
}
