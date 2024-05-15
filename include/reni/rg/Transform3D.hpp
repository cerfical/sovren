#pragma once

#include "../math/types.hpp"
#include "RenderNode.hpp"

namespace reni::rg {

	/**
	 * @brief Matrix to perform affine transformations on 3D geometry nodes.
	*/
	class Transform3D : public RenderNode {
	public:

		Transform3D() = default;

		Transform3D(const Mat4x4& mat) noexcept
			: m_matrix(mat) {}


		void accept(NodeVisitor& visitor) const override;


		void setMatrix(const Mat4x4& mat) noexcept {
			m_matrix = mat;
		}

		const Mat4x4& matrix() const noexcept {
			return m_matrix;
		}


	private:
		Mat4x4 m_matrix = Mat4x4::identity();
	};

}