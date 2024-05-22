#pragma once

#include "../math/Mat3x3.hpp"
#include "../math/Vec2.hpp"

#include "RenderNode.hpp"

#include <optional>

namespace reni::rg {

	/**
	 * @brief Defines an affine transformation to apply to 2D geometry.
	*/
	class Transform2D : public RenderNode {
	public:

		void accept(NodeVisitor& visitor) const override;


		/**
		 * @brief Move the transform along the X axis.
		*/
		void translateX(float dx) noexcept {
			translate(Vec2(dx, 0.0f));
		}


		/**
		 * @brief Move the transform along the Y axis.
		*/
		void translateY(float dy) noexcept {
			translate(Vec2(0.0f, dy));
		}


		/**
		 * @brief Move the transform along the XY axes.
		*/
		void translate(Vec2 dr) noexcept {
			m_pos += dr;
			m_matrix.reset();
		}


		/**
		 * @brief Present the transform in its matrix representation.
		*/
		const Mat3x3& toMatrix() const noexcept {
			if(!m_matrix) {
				m_matrix = Mat3x3::translation(m_pos);
			}
			return *m_matrix;
		}


	private:
		mutable std::optional<Mat3x3> m_matrix;
		Vec2 m_pos;
	};

}