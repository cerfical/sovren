#pragma once

#include <cstddef>

namespace reni {

	struct Vec4 {
		
		float operator[](std::size_t i) const noexcept {
			return (&x)[i];
		}

		float& operator[](std::size_t i) noexcept {
			return (&x)[i];
		}


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

}