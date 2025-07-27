#pragma once

namespace sovren::rhi {

    class Texture {
    public:
        Texture() = default;

        Texture(const Texture&) = delete;
        auto operator=(const Texture&) -> Texture& = delete;

        Texture(Texture&&) = delete;
        auto operator=(Texture&&) -> Texture& = delete;

        virtual ~Texture() = 0;
    };

    inline Texture::~Texture() {}

}
