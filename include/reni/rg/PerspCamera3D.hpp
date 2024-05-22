#pragma once

#include "../math/util.hpp"
#include "Camera3D.hpp"

namespace reni::rg {

	/**
	 * @brief Сreates a perspective projection of a 3D scene onto a 2D plane.
	*/
	class PerspCamera3D : public Camera3D {
	public:

		void setFov(float fov) noexcept {
			m_fov = fov;
			updateProjMatrix();
		}

		float fov() const noexcept {
			return m_fov;
		}

	private:
		Mat4x4 buildProjMatrix() const noexcept override {
			return Mat4x4::perspective(aspectRatio(), fov(), nearPlane(), farPlane());
		}
		
		float m_fov = math::degToRad(45);
	};

}