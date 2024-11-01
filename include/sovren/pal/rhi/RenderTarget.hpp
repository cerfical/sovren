#pragma once

namespace sovren::rhi {

    class RenderTarget {
    public:
        RenderTarget() = default;

        RenderTarget(const RenderTarget&) = delete;
        auto operator=(const RenderTarget&) -> RenderTarget& = delete;

        RenderTarget(RenderTarget&&) = delete;
        auto operator=(RenderTarget&&) -> RenderTarget& = delete;

        virtual ~RenderTarget() = 0;
    };

    inline RenderTarget::~RenderTarget() {}

}
