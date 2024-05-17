#pragma once

#include "../math/types.hpp"
#include "RenderNode.hpp"

namespace reni::rg {

	/**
	 * @brief Defines an affine transformation to apply to 3D geometry.
	*/
	class Transform3D : public RenderNode {
	public:

		/**
		 * @brief Construct an identity transformation.
		*/
		Transform3D() noexcept = default;


		/**
		 * @brief Construct a new transformation from its @ref matrix.
		*/
		Transform3D(const Mat4x4& matrix) noexcept
			: m_matrix(matrix) {}


		void accept(NodeVisitor& visitor) const override;


		/**
		 * @brief Reinitialize the transformation with a new transformation represented as a @ref matrix.
		*/
		void setMatrix(const Mat4x4& matrix) noexcept {
			m_matrix = matrix;
		}


		/**
		 * @brief Matrix representation of the transformation.
		*/
		const Mat4x4& matrix() const noexcept {
			return m_matrix;
		}


		/**
		 * @brief Adjust the absolute position of the transform by the specified displacement.
		*/
		void translate(Vec3 ds) noexcept {
			translate(ds.x, ds.y, ds.z);
		}


		/**
		 * @brief Adjust the absolute position of the transform by the specified displacement values.
		*/
		void translate(float dx, float dy, float dz) noexcept {
			m_matrix *= Mat4x4::translation(dx, dy, dz);
		}


	private:
		Mat4x4 m_matrix = Mat4x4::identity();
	};

}