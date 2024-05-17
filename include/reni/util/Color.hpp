#pragma once

#include "../math/Vec4.hpp"
#include "../math/Vec3.hpp"

#include <compare>

namespace reni {

	/**
	 * @brief RGBA color in the form of four real components.
	*/
	struct Color {
		
		friend auto operator<=>(Color lhs, Color rhs) noexcept = default;


		Color() noexcept
			: Color(0.0, 0.0, 0.0) {}

		Color(Vec4 v) noexcept
			: Color(v.x, v.y, v.z, v.w) {}

		Color(Vec3 v) noexcept
			: Color(v.x, v.y, v.z) {}

		Color(float r, float g, float b, float a = 1.0) noexcept
			: r(r), g(g), b(b), a(a) {}


		operator Vec4() const noexcept {
			return Vec4(r, g, b, a);
		}

		operator Vec3() const noexcept {
			return Vec3(r, g, b);
		}


		float r = {};
		float g = {};
		float b = {};
		float a = {};
	};

}