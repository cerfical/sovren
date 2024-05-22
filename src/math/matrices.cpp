#include "math/Mat4x4.hpp"
#include "math/Mat3x3.hpp"
#include "math/Mat2x2.hpp"

#include "math/ops.hpp"

#include <cmath>

namespace reni {
	namespace {
		// define a trivial singleton matrix to simplify submatrix computation
		struct Vec1 {
			static consteval int order() noexcept { return 1;}

			float operator[](int) const noexcept { return x; }
			float& operator[](int) noexcept { return x; }

			float x = {};
		};

		struct Mat1x1 {
			static consteval int order()  noexcept { return 1; }

			const Vec1& operator[](int) const noexcept { return r.front(); }
			Vec1& operator[](int) noexcept { return r.front(); }
			
			float determinant() const noexcept { return r.front().x; }

			std::array<Vec1, 1> r;
		};


		template <int size> struct SizedMatImpl;

		template <> struct SizedMatImpl<1> {
			using Type = Mat1x1;
		};

		template <> struct SizedMatImpl<2> {
			using Type = Mat2x2;
		};

		template <> struct SizedMatImpl<3> {
			using Type = Mat3x3;
		};

		template <> struct SizedMatImpl<4> {
			using Type = Mat4x4;
		};


		template <int size>
		using SizedMat = SizedMatImpl<size>::Type;



		template <math::Mat SrcMat, math::Mat DestMat = SizedMat<SrcMat::order() - 1>>
		DestMat computeSubmatrix(const SrcMat& srcMat, int row, int col) noexcept {
			// construct a submatrix with values from unneeded row and column removed
			DestMat destMat;
			for(int srcRow = 0, destRow = 0; srcRow < srcMat.order(); srcRow++) {
				// skip the row to exclude from the resulting minor matrix
				if(srcRow == row) {
					continue;
				}

				for(int srcCol = 0, destCol = 0; srcCol < srcMat.order(); srcCol++) {
					// skip the column to exclude from the resulting minor matrix
					if(srcCol == col) {
						continue;
					}
					destMat[destRow][destCol] = srcMat[srcRow][srcCol];
					destCol++;
				}
				destRow++;
			}
			return destMat;
		}


		template <math::Mat Mat>
		float computeMinor(const Mat& m, int row, int col) noexcept {
			return computeSubmatrix(m, row, col).determinant();
		}


		template <math::Mat Mat>
		float computeCofactor(const Mat& m, int row, int col) noexcept {
			return ((row + col) % 2 ? -1 : 1) * computeMinor(m, row, col);
		}


		template <math::Mat Mat>
		float computeDeterminant(const Mat& m) noexcept {
			return fold(m[0], [&m](float v, int j) { return v * computeCofactor(m, 0, j); });
		}


		template <math::Mat Mat>
		Mat computeTranspose(const Mat& m) noexcept {
			return transform(m, [&m](float, int i, int j) { return m[j][i]; });
		}


		template <math::Mat Mat>
		Mat computeAdjugate(const Mat& m) noexcept {
			return transform(m, [&m](float, int i, int j){ return computeCofactor(m, j, i); });
		}
	}
	


	Mat4x4 Mat4x4::perspective(float aspectRatio, float fov, float nearPlane, float farPlane) noexcept {
		const auto c1 = 1.0f / std::tanf(fov / 2.0f);
		const auto c2 = farPlane / (farPlane - nearPlane);
		
		return Mat4x4(
			c1 / aspectRatio, 0.0f,  0.0f,           0.0f,
			0.0f,             c1,    0.0f,           0.0f,
			0.0f,             0.0f,  c2,             1.0f,
			0.0f,             0.0f, -nearPlane * c2, 0.0f
		);
	}

	Mat4x4 Mat4x4::orthographic(Vec2 lensSize, float nearPlane, float farPlane) noexcept {
		const auto c = farPlane - nearPlane;
		return Mat4x4(
			2.0f / lensSize.x, 0.0f,               0.0f,          0.0f,
			0.0f,              2.0f / lensSize.y,  0.0f,          0.0f,
			0.0f,              0.0f,               1.0f / c,      0.0f,
			0.0f,              0.0f,              -nearPlane / c, 1.0f
		);
	}


	Mat4x4 Mat4x4::transpose() const noexcept {
		return computeTranspose(*this);
	}

	Mat4x4 Mat4x4::inverse() const noexcept {
		return computeAdjugate(*this) / determinant();
	}

	float Mat4x4::determinant() const noexcept {
		return computeDeterminant(*this);
	}



	Mat3x3 Mat3x3::transpose() const noexcept {
		return computeTranspose(*this);
	}

	Mat3x3 Mat3x3::inverse() const noexcept {
		return computeAdjugate(*this) / determinant();
	}

	float Mat3x3::determinant() const noexcept {
		return computeDeterminant(*this);
	}



	Mat2x2 Mat2x2::transpose() const noexcept {
		return computeTranspose(*this);
	}

	Mat2x2 Mat2x2::inverse() const noexcept {
		return computeAdjugate(*this) / determinant();
	}

	float Mat2x2::determinant() const noexcept {
		return computeDeterminant(*this);
	}
}