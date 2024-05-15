#pragma once

#include "types.hpp"

namespace reni {

	inline Vec2 operator+(Vec2 lhs, Vec2 rhs) noexcept {
		return Vec2(
			lhs.x + rhs.x,
			lhs.y + rhs.y
		);
	}

	Vec2& Vec2::operator+=(Vec2 rhs) noexcept {
		return *this = *this + rhs;
	}

	inline Vec2 operator-(Vec2 lhs, Vec2 rhs) noexcept {
		return Vec2(
			lhs.x - rhs.x,
			lhs.y - rhs.y
		);
	}

	Vec2& Vec2::operator-=(Vec2 rhs) noexcept {
		return *this = *this - rhs;
	}



	inline Vec3 operator+(Vec3 lhs, Vec3 rhs) noexcept {
		return Vec3(
			lhs.x + rhs.x,
			lhs.y + rhs.y,
			lhs.z + rhs.z
		);
	}

	Vec3& Vec3::operator+=(Vec3 rhs) noexcept {
		return *this = *this + rhs;
	}

	inline Vec3 operator-(Vec3 lhs, Vec3 rhs) noexcept {
		return Vec3(
			lhs.x - rhs.x,
			lhs.y - rhs.y,
			lhs.z - rhs.z
		);
	}

	Vec3& Vec3::operator-=(Vec3 rhs) noexcept {
		return *this = *this - rhs;
	}



	inline Vec4 operator+(Vec4 lhs, Vec4 rhs) noexcept {
		return Vec4(
			lhs.x + rhs.x,
			lhs.y + rhs.y,
			lhs.z + rhs.z,
			lhs.w + rhs.w
		);
	}

	Vec4& Vec4::operator+=(Vec4 rhs) noexcept {
		return *this = *this + rhs;
	}

	inline Vec4 operator-(Vec4 lhs, Vec4 rhs) noexcept {
		return Vec4(
			lhs.x - rhs.x,
			lhs.y - rhs.y,
			lhs.z - rhs.z,
			lhs.w - rhs.w
		);
	}

	Vec4& Vec4::operator-=(Vec4 rhs) noexcept {
		return *this = *this - rhs;
	}



	inline Mat4x4 operator+(const Mat4x4& lhs, const Mat4x4& rhs) noexcept {
		return Mat4x4(
			lhs.r1 + rhs.r1,
			lhs.r2 + rhs.r2,
			lhs.r3 + rhs.r3,
			lhs.r4 + rhs.r4
		);
	}

	Mat4x4& Mat4x4::operator+=(const Mat4x4& rhs) noexcept {
		return *this = *this + rhs;
	}

	inline Mat4x4 operator-(const Mat4x4& lhs, const Mat4x4& rhs) noexcept {
		return Mat4x4(
			lhs.r1 - rhs.r1,
			lhs.r2 - rhs.r2,
			lhs.r3 - rhs.r3,
			lhs.r4 - rhs.r4
		);
	}

	Mat4x4& Mat4x4::operator-=(const Mat4x4& rhs) noexcept {
		return *this = *this - rhs;
	}



	inline Mat3x3 operator+(const Mat3x3& lhs, const Mat3x3& rhs) noexcept {
		return Mat3x3(
			lhs.r1 + rhs.r1,
			lhs.r2 + rhs.r2,
			lhs.r3 + rhs.r3
		);
	}

	Mat3x3& Mat3x3::operator+=(const Mat3x3& rhs) noexcept {
		return *this = *this + rhs;
	}

	inline Mat3x3 operator-(const Mat3x3& lhs, const Mat3x3& rhs) noexcept {
		return Mat3x3(
			lhs.r1 - rhs.r1,
			lhs.r2 - rhs.r2,
			lhs.r3 - rhs.r3
		);
	}

	Mat3x3& Mat3x3::operator-=(const Mat3x3& rhs) noexcept {
		return *this = *this - rhs;
	}



	inline Mat2x2 operator+(const Mat2x2& lhs, const Mat2x2& rhs) noexcept {
		return Mat2x2(
			lhs.r1 + rhs.r1,
			lhs.r2 + rhs.r2
		);
	}

	Mat2x2& Mat2x2::operator+=(const Mat2x2& rhs) noexcept {
		return *this = *this + rhs;
	}

	inline Mat2x2 operator-(const Mat2x2& lhs, const Mat2x2& rhs) noexcept {
		return Mat2x2(
			lhs.r1 - rhs.r1,
			lhs.r2 - rhs.r2
		);
	}

	Mat2x2& Mat2x2::operator-=(const Mat2x2& rhs) noexcept {
		return *this = *this - rhs;
	}

}
