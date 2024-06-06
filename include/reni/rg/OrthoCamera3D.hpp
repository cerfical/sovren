#pragma once

#include "Camera3D.hpp"

namespace reni::rg {

	/**
	 * @brief Сreates an orthographic projection of a 3D scene onto a 2D plane.
	*/
	class OrthoCamera3D : public Camera3D {
	private:
		Mat4x4 buildProjMatrix() const noexcept override {
			const auto s = lensSize();
			return Mat4x4::orthographic(s.x, s.y, nearPlane(), farPlane());
		}
	};

}