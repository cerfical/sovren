#pragma once

#include <cstddef>

namespace reni {

	struct Vec2 {
		
		float operator[](std::size_t i) const noexcept {
			return (&x)[i];
		}

		float& operator[](std::size_t i) noexcept {
			return (&x)[i];
		}


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

}