#pragma once

#include <cstddef>
#include <compare>

namespace reni {

	struct Vec4 {
		
		friend auto operator<=>(Vec4, Vec4) noexcept = default;


		inline float operator[](std::size_t i) const noexcept;

		inline float& operator[](std::size_t i) noexcept;


		inline Vec4& operator+=(Vec4 rhs) noexcept;
		
		inline Vec4& operator-=(Vec4 rhs) noexcept;


		float x = {};
		float y = {};
		float z = {};
		float w = 1.0f;
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


	float Vec4::operator[](std::size_t i) const noexcept {
		return begin(*this)[i];
	}

	float& Vec4::operator[](std::size_t i) noexcept {
		return begin(*this)[i];
	}

}