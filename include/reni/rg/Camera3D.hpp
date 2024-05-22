#pragma once

#include "../math/Mat4x4.hpp"
#include "../math/Vec2.hpp"

#include "RenderNode.hpp"

#include <optional>

namespace reni::rg {

	/**
	 * @brief Presents 3D objects in a 2D plane using some projection matrix.
	*/
	class Camera3D : public RenderNode {
	public:

		void accept(NodeVisitor& visitor) const override;


		void setNearPlane(float nearPlane) noexcept {
			m_nearPlane = nearPlane;
			updateProjMatrix();
		}

		float nearPlane() const noexcept {
			return m_nearPlane;
		}


		void setFarPlane(float farPlane) noexcept {
			m_farPlane = farPlane;
			updateProjMatrix();
		}

		float farPlane() const noexcept {
			return m_farPlane;
		}


		void setLensSize(Vec2 lensSize) noexcept {
			m_lensSize = lensSize;
			updateProjMatrix();
		}

		Vec2 lensSize() const noexcept {
			return m_lensSize;
		}

		float aspectRatio() const noexcept {
			return m_lensSize.x / m_lensSize.y;
		}


		const Mat4x4& toProjMatrix() const noexcept {
			if(!m_projMatrix) {
				m_projMatrix = buildProjMatrix();
			}
			return *m_projMatrix;
		}


	protected:
		void updateProjMatrix() noexcept {
			m_projMatrix.reset();
		}

	private:
		virtual Mat4x4 buildProjMatrix() const noexcept = 0;

		mutable std::optional<Mat4x4> m_projMatrix;

		Vec2 m_lensSize = { 1.0f, 1.0f };
		float m_farPlane = 1000.0f;
		float m_nearPlane = 1.0f;
	};

}