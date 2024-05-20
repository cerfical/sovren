#pragma once

#include "../math/Mat4x4.hpp"
#include "RenderNode.hpp"

namespace reni::rg {

	/**
	 * @brief Performs projection of 3D objects onto a 2D plane.
	*/
	class CameraNode : public RenderNode {
	public:

		/**
		 * @brief Construct a camera with an identity projection.
		*/
		CameraNode() noexcept = default;
		

		/**
		 * @brief Construct a new camera described by its projection matrix.
		*/
		CameraNode(const Mat4x4& mat) noexcept
			: m_projMatrix(mat) {}


		void accept(NodeVisitor& visitor) const override;


		/**
		 * @brief Reinitialize the camera with a new projection matrix.
		*/
		void setProjMatrix(const Mat4x4& mat) noexcept {
			m_projMatrix = mat;
		}


		/**
		 * @brief The projection matrix defining the camera.
		*/
		const Mat4x4& projMatrix() const noexcept {
			return m_projMatrix;
		}


	private:
		Mat4x4 m_projMatrix = Mat4x4::identity();
	};

}