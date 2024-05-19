#include "math/Mat4x4.hpp"
#include "math/Mat3x3.hpp"
#include "math/Mat2x2.hpp"
#include "math/ops.hpp"

namespace reni {
	namespace {
		// define a trivial singleton matrix to simplify submatrix computation
		struct Vec1 {
			static consteval int len() noexcept { return 1;}

			float operator[](int) const noexcept { return x; }
			float& operator[](int) noexcept { return x; }

			float x = {};
		};

		struct Mat1x1 {
			static consteval int rowCount() noexcept { return 1; }
			static consteval int colCount()  noexcept { return 1; }

			Vec1 operator[](int) const noexcept { return r1; }
			Vec1& operator[](int) noexcept { return r1; }
			
			float determinant() const noexcept { return r1.x; }

			Vec1 r1;
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



		template <math::Mat SrcMat, math::Mat DestMat = SizedMat<SrcMat::rowCount() - 1>>
		DestMat computeSubmatrix(const SrcMat& srcMat, int row, int col) noexcept {
			// construct a submatrix with values from unneeded row and column removed
			DestMat destMat;
			for(int srcRow = 0, destRow = 0; srcRow < srcMat.rowCount(); srcRow++) {
				// skip the row to exclude from the resulting minor matrix
				if(srcRow == row) {
					continue;
				}

				for(int srcCol = 0, destCol = 0; srcCol < srcMat.colCount(); srcCol++) {
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



	Mat4x4 Mat4x4::transpose() const noexcept {
		return computeTranspose(*this);
	}

	Mat3x3 Mat3x3::transpose() const noexcept {
		return computeTranspose(*this);
	}

	Mat2x2 Mat2x2::transpose() const noexcept {
		return computeTranspose(*this);
	}

	
	Mat4x4 Mat4x4::inverse() const noexcept {
		return computeAdjugate(*this) / determinant();
	}
	
	Mat3x3 Mat3x3::inverse() const noexcept {
		return computeAdjugate(*this) / determinant();
	}

	Mat2x2 Mat2x2::inverse() const noexcept {
		return computeAdjugate(*this) / determinant();
	}


	float Mat4x4::determinant() const noexcept {
		return computeDeterminant(*this);
	}

	float Mat3x3::determinant() const noexcept {
		return computeDeterminant(*this);
	}

	float Mat2x2::determinant() const noexcept {
		return computeDeterminant(*this);
	}
}