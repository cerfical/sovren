#pragma once

#include "Vec4.hpp"
#include "Vec3.hpp"
#include "util.hpp"

#include <cmath>
#include <cstddef>

namespace reni {

	struct Mat4x4 {

		friend bool operator==(const Mat4x4&, const Mat4x4&) noexcept = default;


		static constexpr float ProjFov = math::degToRad(90);
		static constexpr float ProjNearPlane = 1;
		static constexpr float ProjFarPlane = 1000;


		static Mat4x4 identity() noexcept {
			return Mat4x4(
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			);
		}


		static Mat4x4 translation(Vec3 ds) noexcept {
			return translation(ds.x, ds.y, ds.z);
		}

		static Mat4x4 translation(float dx, float dy, float dz) noexcept {
			return Mat4x4(
				1,  0,  0,  0,
				0,  1,  0,  0,
				0,  0,  1,  0,
				dx, dy, dz, 1
			);
		}


		static Mat4x4 perspective(
			float aspectRatio,
			float fovRad = ProjFov,
			float nearPlane = ProjNearPlane,
			float farPlane = ProjFarPlane
		) noexcept {
			const auto c1 = 1.0f / std::tan(fovRad / 2.0f);
			const auto c2 = farPlane / (farPlane - nearPlane);
			
			return Mat4x4(
				c1 / aspectRatio, 0,   0,              0,
				0,                c1,  0,              0,
				0,                0,   c2,             1,
				0,                0,  -nearPlane * c2, 0
			);
		}


		static Mat4x4 orthographic(
			float width,
			float height,
			float nearPlane = ProjNearPlane,
			float farPlane = ProjFarPlane
		) noexcept {
			const auto c = farPlane - nearPlane;
			return Mat4x4(
				2.0f / width, 0,              0,             0,
				0,            2.0f / height,  0,             0,
				0,            0,              1.0f / c,      0,
				0,            0,             -nearPlane / c, 1
			);
		}


		static consteval int rowCount() noexcept {
			return 4;
		}

		static consteval int colCount() noexcept  {
			return 4;
		}


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
			: r1(r1), r2(r2), r3(r3), r4(r4) {}


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


		Vec4 r1;
		Vec4 r2;
		Vec4 r3;
		Vec4 r4;
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


	const Vec4& Mat4x4::operator[](int i) const noexcept {
		return begin(*this)[i];
	}

	Vec4& Mat4x4::operator[](int i) noexcept {
		return begin(*this)[i];
	}

}