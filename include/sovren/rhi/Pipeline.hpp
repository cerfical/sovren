#pragma once

namespace sovren::rhi {

    class Pipeline {
    public:
        Pipeline() = default;

        Pipeline(const Pipeline&) = delete;
        auto operator=(const Pipeline&) -> Pipeline& = delete;

        Pipeline(Pipeline&&) = delete;
        auto operator=(Pipeline&&) -> Pipeline& = delete;

        virtual ~Pipeline() = 0;
    };

    inline Pipeline::~Pipeline() {}

}
