#pragma once

#include "../math/types.hpp"
#include "RenderNode.hpp"

namespace reni::rg {

	/**
	 * @brief Matrix to perform affine transformations on 2D geometry nodes.
	*/
	class Transform2D : public RenderNode {
	public:

		Transform2D() = default;

		Transform2D(const Mat3x3& mat) noexcept
			: m_matrix(mat) {}


		void accept(NodeVisitor& visitor) const override;


		void setMatrix(const Mat3x3& mat) noexcept {
			m_matrix = mat;
		}

		const Mat3x3& matrix() const noexcept {
			return m_matrix;
		}


	private:
		Mat3x3 m_matrix = Mat3x3::identity();
	};

}