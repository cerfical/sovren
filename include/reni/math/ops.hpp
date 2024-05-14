#pragma once

#include "types.hpp"

namespace reni {

	inline Vec2 operator+(Vec2 lhs, Vec2 rhs) noexcept {
		return Vec2(lhs.x + rhs.x, lhs.y + rhs.y);
	}

	Vec2& Vec2::operator+=(Vec2 rhs) noexcept {
		return *this = *this + rhs;
	}

	inline Vec2 operator-(Vec2 lhs, Vec2 rhs) noexcept {
		return Vec2(lhs.x - rhs.x, lhs.y - rhs.y);
	}

	Vec2& Vec2::operator-=(Vec2 rhs) noexcept {
		return *this = *this - rhs;
	}



	inline Vec3 operator+(Vec3 lhs, Vec3 rhs) noexcept {
		return Vec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
	}

	Vec3& Vec3::operator+=(Vec3 rhs) noexcept {
		return *this = *this + rhs;
	}

	inline Vec3 operator-(Vec3 lhs, Vec3 rhs) noexcept {
		return Vec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
	}

	Vec3& Vec3::operator-=(Vec3 rhs) noexcept {
		return *this = *this - rhs;
	}



	inline Vec4 operator+(Vec4 lhs, Vec4 rhs) noexcept {
		return Vec4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
	}

	Vec4& Vec4::operator+=(Vec4 rhs) noexcept {
		return *this = *this + rhs;
	}

	inline Vec4 operator-(Vec4 lhs, Vec4 rhs) noexcept {
		return Vec4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
	}

	Vec4& Vec4::operator-=(Vec4 rhs) noexcept {
		return *this = *this - rhs;
	}

}
