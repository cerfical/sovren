#pragma once

namespace sovren {

    class RenderTarget {
    public:
        RenderTarget(const RenderTarget&) = delete;
        RenderTarget& operator=(const RenderTarget&) = delete;

        RenderTarget(RenderTarget&&) = delete;
        RenderTarget& operator=(RenderTarget&&) = delete;

        RenderTarget() = default;
        virtual ~RenderTarget() = 0;
    };

    inline RenderTarget::~RenderTarget() = default;

}
