#pragma once

#include "Vec3.hpp"
#include <compare>

namespace reni {

	struct Mat4x4;


	struct Vec4 {
		
		friend auto operator<=>(Vec4, Vec4) noexcept = default;


		static consteval int order() noexcept {
			return 4;
		}


		static Vec4 splat(float v) noexcept {
			return Vec4(v, v, v, v);
		}


		Vec4() noexcept = default;

		Vec4(float x, float y, float z, float w) noexcept
			: x(x), y(y), z(z), w(w) {}


		inline float operator[](int i) const noexcept;

		inline float& operator[](int i) noexcept;


		inline Vec4& operator+=(Vec4 rhs) noexcept;
		
		inline Vec4& operator-=(Vec4 rhs) noexcept;

		inline Vec4& operator*=(Vec4 rhs) noexcept;

		inline Vec4& operator/=(Vec4 rhs) noexcept;


		inline Vec4& operator+=(float rhs) noexcept;
		
		inline Vec4& operator-=(float rhs) noexcept;
		
		inline Vec4& operator*=(float rhs) noexcept;
		
		inline Vec4& operator/=(float rhs) noexcept;


		inline Vec4& operator*=(const Mat4x4& rhs) noexcept;


		operator Vec3() const noexcept {
			return Vec3(x, y, z);
		}


		float x = {};
		float y = {};
		float z = {};
		float w = {};
	};


	inline const float* begin(const Vec4& v) noexcept {
		return &v.x;
	}

	inline float* begin(Vec4& v) noexcept {
		return &v.x;
	}


	inline const float* end(const Vec4& v) noexcept {
		return &v.w + 1;
	}

	inline float* end(Vec4& v) noexcept {
		return &v.w + 1;
	}


	float Vec4::operator[](int i) const noexcept {
		return begin(*this)[i];
	}

	float& Vec4::operator[](int i) noexcept {
		return begin(*this)[i];
	}

}