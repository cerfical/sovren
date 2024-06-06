#pragma once

#include "../math/Mat4x4.hpp"
#include "../math/Vec3.hpp"

#include "RenderNode.hpp"

#include <optional>

namespace reni::rg {

	/**
	 * @brief Defines an affine transformation to apply to 3D geometry.
	*/
	class Transform3D : public RenderNode {
	public:

		void accept(NodeVisitor& visitor) const override;


		/**
		 * @brief Move the transform along the X axis.
		*/
		void translateX(float dx) noexcept {
			translate(Vec3(dx, 0.0f, 0.0f));
		}


		/**
		 * @brief Move the transform along the Y axis.
		*/
		void translateY(float dy) noexcept {
			translate(Vec3(0.0f, dy, 0.0f));
		}


		/**
		 * @brief Move the transform along the Z axis.
		*/
		void translateZ(float dz) noexcept {
			translate(Vec3(0.0f, 0.0f, dz));
		}


		/**
		 * @brief Move the transform along the XYZ axes.
		*/
		void translate(Vec3 dr) noexcept {
			m_pos += dr;
			m_matrix.reset();
		}


		/**
		 * @brief Present the transform in its matrix representation.
		*/
		const Mat4x4& toMatrix() const noexcept {
			if(!m_matrix) {
				m_matrix = Mat4x4::translation(m_pos.x, m_pos.y, m_pos.z);
			}
			return *m_matrix;
		}


	private:
		mutable std::optional<Mat4x4> m_matrix;
		Vec3 m_pos;
	};

}