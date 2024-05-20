#pragma once

#include "../util/types.hpp"
#include "../math/types.hpp"

namespace reni::rhi {

	class RenderTarget;
	class VertexBuffer;


	class RenderContext : private NonCopyable, private NonMovable {
	public:

		virtual ~RenderContext() = default;


		virtual void startRender(RenderTarget& rt) = 0;
	
		virtual void endRender() = 0;


		virtual void drawLine(Vec2 start, Vec2 end) = 0;

		virtual void drawRect(Vec2 topLeft, Vec2 bottomRight) = 0;

		virtual void drawMesh(const VertexBuffer& vert) = 0;

		virtual void clear(Color col) = 0;


		virtual void setTransformMatrix(const Mat4x4& mat) = 0;

		virtual void setTransformMatrix(const Mat3x3& mat) = 0;

	};

}