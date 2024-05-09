#pragma once

#include <cstddef>

namespace reni {

	struct Vec3 {
		
		float operator[](std::size_t i) const noexcept {
			return (&x)[i];
		}

		float& operator[](std::size_t i) noexcept {
			return (&x)[i];
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

}