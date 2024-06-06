#pragma once

#include "Mat.hpp"

#include <iterator>
#include <type_traits>

namespace reni::math {

    template <typename Mat, typename Sub, typename Vec>
    struct MatBase {

        friend bool operator==(MatBase, MatBase) noexcept = default;


        friend const Vec* begin(const MatBase& m) noexcept {
            return begin(const_cast<MatBase&>(m));
        }

        friend Vec* begin(MatBase& m) noexcept {
            return &m.asMat()[0];
        }


        friend const Vec* end(const MatBase& m) noexcept {
            return end(const_cast<MatBase&>(m));
        }

        friend Vec* end(MatBase& m) noexcept {
            return std::next(&m.asMat()[Mat::Order - 1]);
        }


        static Mat identity() noexcept {
            Mat mat;
            for(int i = 0; i < Mat::Order; i++) {
                mat[i][i] = 1.0f;
            }
            return mat;
        }


        Mat transpose() const noexcept {
            return transform(asMat(), [this](float, int i, int j) { return asMat()[j][i]; });
        }

        float determinant() const noexcept {
            return fold(asMat()[0], [this](float v, int j) { return v * cofactor(0, j); });
        }

        Mat inverse() const noexcept {
            return adjugate() / determinant();
        }


        Mat& operator+=(const Mat& rhs) noexcept {
            return asMat() = asMat() + rhs;
        }

        Mat& operator-=(const Mat& rhs) noexcept {
            return asMat() = asMat() - rhs;
        }

        Mat& operator*=(const Mat& rhs) noexcept {
            return asMat() = asMat() * rhs;
        }


        Mat& operator+=(float rhs) noexcept {
            return asMat() = asMat() + rhs;
        }

        Mat& operator-=(float rhs) noexcept {
            return asMat() = asMat() - rhs;
        }

        Mat& operator*=(float rhs) noexcept {
            return asMat() = asMat() * rhs;
        }

        Mat& operator/=(float rhs) noexcept {
            return asMat() = asMat() / rhs;
        }


    private:
        const Mat& asMat() const noexcept {
            return const_cast<MatBase&>(*this).asMat();
        }

        Mat& asMat() noexcept {
            return static_cast<Mat&>(*this);
        }


        Mat adjugate() const noexcept {
            return transform(asMat(), [this](float, int i, int j) { return cofactor(j, i); });
        }

        float cofactor(int row, int col) const noexcept {
            return (((row + col) % 2 == 0) ? 1 : -1) * minor(row, col);
        }

        float minor(int row, int col) const noexcept {
            return submatrix(row, col).determinant();
        }


        // trivial singleton matrix definition to simplify submatrix computation
        struct Vec1 {
            static inline constexpr int Order = 1;

            float& operator[](int) noexcept {
                return x;
            }

            float x = {};
        };

        struct Mat1x1 {
            static inline constexpr int Order = 1;

            Vec1& operator[](int) noexcept {
                return row;
            }

            float determinant() const noexcept {
                return row.x;
            }

            Vec1 row;
        };


        using SubmatrixType = std::conditional_t<math::Mat<Sub>, Sub, Mat1x1>;

        SubmatrixType submatrix(int row, int col) const noexcept {
            SubmatrixType destMat;
            for(int srcRow = 0, destRow = 0; srcRow < Mat::Order; srcRow++) {
                // skip the row to exclude from the resulting submatrix
                if(srcRow == row) {
                    continue;
                }

                for(int srcCol = 0, destCol = 0; srcCol < Mat::Order; srcCol++) {
                    // skip the column to exclude from the resulting submatrix
                    if(srcCol == col) {
                        continue;
                    }
                    destMat[destRow][destCol] = asMat()[srcRow][srcCol];
                    destCol++;
                }
                destRow++;
            }
            return destMat;
        }
    };

}
