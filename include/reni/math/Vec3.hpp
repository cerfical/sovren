#pragma once

#include "Vec2.hpp"

#include <cstddef>
#include <compare>

namespace reni {

	struct Mat3x3;


	struct Vec3 {
		
		friend auto operator<=>(Vec3, Vec3) noexcept = default;


		static Vec3 splat(float v) noexcept {
			return Vec3(v, v, v);
		}

		static consteval int len() noexcept {
			return 3;
		}


		Vec3() noexcept = default;

		Vec3(float x, float y, float z) noexcept
			: x(x), y(y), z(z) {}


		inline float operator[](int i) const noexcept;

		inline float& operator[](int i) noexcept;


		inline Vec3& operator+=(Vec3 rhs) noexcept;
		
		inline Vec3& operator-=(Vec3 rhs) noexcept;

		inline Vec3& operator*=(Vec3 rhs) noexcept;

		inline Vec3& operator/=(Vec3 rhs) noexcept;


		inline Vec3& operator+=(float rhs) noexcept;
		
		inline Vec3& operator-=(float rhs) noexcept;
		
		inline Vec3& operator*=(float rhs) noexcept;
		
		inline Vec3& operator/=(float rhs) noexcept;


		inline Vec3& operator*=(const Mat3x3& rhs) noexcept;


		operator Vec2() const noexcept {
			return Vec2(x, y);
		}


		float x = {};
		float y = {};
		float z = {};
	};


	inline const float* begin(const Vec3& v) noexcept {
		return &v.x;
	}

	inline float* begin(Vec3& v) noexcept {
		return &v.x;
	}


	inline const float* end(const Vec3& v) noexcept {
		return &v.z + 1;
	}

	inline float* end(Vec3& v) noexcept {
		return &v.z + 1;
	}


	float Vec3::operator[](int i) const noexcept {
		return begin(*this)[i];
	}

	float& Vec3::operator[](int i) noexcept {
		return begin(*this)[i];
	}

}