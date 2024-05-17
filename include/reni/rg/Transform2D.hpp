#pragma once

#include "../math/types.hpp"
#include "RenderNode.hpp"

namespace reni::rg {

	/**
	 * @brief Defines an affine transformation to apply to 2D geometry.
	*/
	class Transform2D : public RenderNode {
	public:

		/**
		 * @brief Construct an identity transformation.
		*/
		Transform2D() noexcept = default;


		/**
		 * @brief Construct a new transformation from its @ref matrix.
		*/
		Transform2D(const Mat3x3& matrix) noexcept
			: m_matrix(matrix) {}


		void accept(NodeVisitor& visitor) const override;


		/**
		 * @brief Reinitialize the transformation with a new transformation represented as a @ref matrix.
		*/
		void setMatrix(const Mat3x3& matrix) noexcept {
			m_matrix = matrix;
		}


		/**
		 * @brief Matrix representation of the transformation.
		*/
		const Mat3x3& matrix() const noexcept {
			return m_matrix;
		}


		/**
		 * @brief Adjust the absolute position of the transform by the specified displacement.
		*/
		void translate(Vec2 ds) noexcept {
			translate(ds.x, ds.y);
		}


		/**
		 * @brief Adjust the absolute position of the transform by the specified displacement values.
		*/
		void translate(float dx, float dy) noexcept {
			m_matrix *= Mat3x3::translation(dx, dy);
		}


	private:
		Mat3x3 m_matrix = Mat3x3::identity();
	};

}