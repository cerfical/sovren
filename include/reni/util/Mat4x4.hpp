#pragma once

#include "Vec4.hpp"
#include <cstddef>

namespace reni {

	struct Mat4x4 {

		const Vec4& operator[](std::size_t i) const noexcept {
			return (&r1)[i];
		}

		Vec4& operator[](std::size_t i) noexcept {
			return (&r1)[i];
		}


		Vec4 r1 = {};
		Vec4 r2 = {};
		Vec4 r3 = {};
		Vec4 r4 = {};
	};



	inline const Vec4* begin(const Mat4x4& m) noexcept {
		return &m.r1;
	}

	inline Vec4* begin(Mat4x4& m) noexcept {
		return &m.r1;
	}


	inline const Vec4* end(const Mat4x4& m) noexcept {
		return &m.r4 + 1;
	}

	inline Vec4* end(Mat4x4& m) noexcept {
		return &m.r4 + 1;
	}

}