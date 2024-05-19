#pragma once

#include <cstddef>
#include <compare>

namespace reni {

	struct Mat2x2;


	struct Vec2 {

		friend auto operator<=>(Vec2, Vec2) noexcept = default;


		static Vec2 splat(float v) noexcept {
			return Vec2(v, v);
		}

		static consteval int len() noexcept {
			return 2;
		}


		Vec2() noexcept = default;

		Vec2(float x, float y) noexcept
			: x(x), y(y) {}


		inline float operator[](int i) const noexcept;

		inline float& operator[](int i) noexcept;


		inline Vec2& operator+=(Vec2 rhs) noexcept;
		
		inline Vec2& operator-=(Vec2 rhs) noexcept;

		inline Vec2& operator*=(Vec2 rhs) noexcept;

		inline Vec2& operator/=(Vec2 rhs) noexcept;
		

		inline Vec2& operator+=(float rhs) noexcept;
		
		inline Vec2& operator-=(float rhs) noexcept;
		
		inline Vec2& operator*=(float rhs) noexcept;
		
		inline Vec2& operator/=(float rhs) noexcept;


		inline Vec2& operator*=(const Mat2x2& rhs) noexcept;


		float x = {};
		float y = {};
	};


	inline const float* begin(const Vec2& v) noexcept {
		return &v.x;
	}

	inline float* begin(Vec2& v) noexcept {
		return &v.x;
	}


	inline const float* end(const Vec2& v) noexcept {
		return &v.y + 1;
	}

	inline float* end(Vec2& v) noexcept {
		return &v.y + 1;
	}


	float Vec2::operator[](int i) const noexcept {
		return begin(*this)[i];
	}

	float& Vec2::operator[](int i) noexcept {
		return begin(*this)[i];
	}

}