#pragma once

#include <cstddef>
#include <compare>

namespace reni {

	struct Vec2 {

		friend auto operator<=>(Vec2, Vec2) noexcept = default;


		inline float operator[](std::size_t i) const noexcept;

		inline float& operator[](std::size_t i) noexcept;


		inline Vec2& operator+=(Vec2 rhs) noexcept;
		
		inline Vec2& operator-=(Vec2 rhs) noexcept;


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


	float Vec2::operator[](std::size_t i) const noexcept {
		return begin(*this)[i];
	}

	float& Vec2::operator[](std::size_t i) noexcept {
		return begin(*this)[i];
	}

}