#pragma once

#include "Vec4.hpp"
#include "Vec3.hpp"
#include "Vec2.hpp"

#include <array>

namespace reni {

	struct Mat4x4 {

		friend bool operator==(const Mat4x4&, const Mat4x4&) noexcept = default;


		static consteval int order() noexcept {
			return static_cast<int>(decltype(r)().size());
		}


		static Mat4x4 identity() noexcept {
			return Mat4x4(
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			);
		}


		static Mat4x4 translation(Vec3 dr) noexcept {
			return Mat4x4(
				1.0f,  0.0f,  0.0f,  0.0f,
				0.0f,  1.0f,  0.0f,  0.0f,
				0.0f,  0.0f,  1.0f,  0.0f,
				dr.x,  dr.y,  dr.z,  1.0f
			);
		}


		static Mat4x4 perspective(float aspectRatio, float fov, float nearPlane, float farPlane) noexcept;

		static Mat4x4 orthographic(Vec2 lensSize, float nearPlane, float farPlane) noexcept;


		Mat4x4() noexcept = default;

		Mat4x4(
			float x1, float y1, float z1, float w1,
			float x2, float y2, float z2, float w2,
			float x3, float y3, float z3, float w3,
			float x4, float y4, float z4, float w4
		) noexcept
			: Mat4x4(
				Vec4(x1, y1, z1, w1),
				Vec4(x2, y2, z2, w2),
				Vec4(x3, y3, z3, w3),
				Vec4(x4, y4, z4, w4)
			) {}

		Mat4x4(Vec4 r1, Vec4 r2, Vec4 r3, Vec4 r4) noexcept
			: r({ r1, r2, r3, r4 }) {}


		inline const Vec4& operator[](int i) const noexcept;

		inline Vec4& operator[](int i) noexcept;


		inline Mat4x4& operator+=(const Mat4x4& rhs) noexcept;
		
		inline Mat4x4& operator-=(const Mat4x4& rhs) noexcept;

		inline Mat4x4& operator*=(const Mat4x4& rhs) noexcept;


		inline Mat4x4& operator+=(float rhs) noexcept;
		
		inline Mat4x4& operator-=(float rhs) noexcept;
		
		inline Mat4x4& operator*=(float rhs) noexcept;
		
		inline Mat4x4& operator/=(float rhs) noexcept;


		Mat4x4 transpose() const noexcept;

		Mat4x4 inverse() const noexcept;

		float determinant() const noexcept;

		std::array<Vec4, 4> r;
	};



	inline const Vec4* begin(const Mat4x4& m) noexcept {
		return m.r.data();
	}

	inline Vec4* begin(Mat4x4& m) noexcept {
		return m.r.data();
	}


	inline const Vec4* end(const Mat4x4& m) noexcept {
		return m.r.data() + m.r.size();
	}

	inline Vec4* end(Mat4x4& m) noexcept {
		return m.r.data() + m.r.size();
	}


	const Vec4& Mat4x4::operator[](int i) const noexcept {
		return begin(*this)[i];
	}

	Vec4& Mat4x4::operator[](int i) noexcept {
		return begin(*this)[i];
	}

}