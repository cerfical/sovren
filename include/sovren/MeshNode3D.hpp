#pragma once

#include "rhi/Buffer.hpp"
#include "rhi/Pipeline.hpp"

#include "Mesh.hpp"
#include "SceneNode3D.hpp"

#include <memory>
#include <utility>

namespace sovren {

    class MeshNode3D : public SceneNode3D {
    public:
        explicit MeshNode3D(Mesh mesh) noexcept
            : mesh_(std::move(mesh)) {}

        void onRender() override;

    private:
        std::unique_ptr<rhi::Buffer> buffer_;
        std::unique_ptr<rhi::Pipeline> pipeline_;

        Mesh mesh_;
    };

}
