#pragma once

#include <array>
#include <iterator>

namespace sovren::math {

    template <typename Mat, typename Vec, int Ord>
    struct MatBase {

        static inline constexpr int Order = Ord;


        friend bool operator==(const Mat& lhs, const Mat& rhs) noexcept {
            for(int i = 0; i < Order; i++) {
                if(lhs[i] != rhs[i]) {
                    return false;
                }
            }
            return true;
        }


        friend Mat operator+(Mat lhs, const Mat& rhs) noexcept {
            return lhs += rhs;
        }

        friend Mat operator-(Mat lhs, const Mat& rhs) noexcept {
            return lhs -= rhs;
        }

        friend Mat operator*(Mat lhs, const Mat& rhs) noexcept {
            return lhs *= rhs;
        }


        friend Mat operator+(Mat lhs, float rhs) noexcept {
            return lhs += rhs;
        }

        friend Mat operator-(Mat lhs, float rhs) noexcept {
            return lhs -= rhs;
        }

        friend Mat operator*(Mat lhs, float rhs) noexcept {
            return lhs *= rhs;
        }

        friend Mat operator/(Mat lhs, float rhs) noexcept {
            return lhs /= rhs;
        }


        friend Mat& operator*=(Mat& lhs, const Mat& rhs) noexcept {
            for(int i = 0; i < Order; i++) {
                lhs[i] *= rhs;
            }
            return lhs;
        }


        friend Mat& operator+=(Mat& lhs, const Mat& rhs) noexcept {
            return lhs.combine(rhs, [](auto& l, auto r) { l += r; });
        }

        friend Mat& operator-=(Mat& lhs, const Mat& rhs) noexcept {
            return lhs.combine(rhs, [](auto& l, auto r) { l -= r; });
        }


        friend Mat& operator+=(Mat& lhs, float rhs) noexcept {
            return lhs.transform([rhs](auto& l) { l += rhs; });
        }

        friend Mat& operator-=(Mat& lhs, float rhs) noexcept {
            return lhs.transform([rhs](auto& l) { l -= rhs; });
        }

        friend Mat& operator*=(Mat& lhs, float rhs) noexcept {
            return lhs.transform([rhs](auto& l) { l *= rhs; });
        }

        friend Mat& operator/=(Mat& lhs, float rhs) noexcept {
            return lhs.transform([rhs](auto& l) { l /= rhs; });
        }


        friend const Vec* begin(const Mat& m) noexcept {
            return begin(const_cast<Mat&>(m));
        }

        friend const Vec* end(const Mat& m) noexcept {
            return end(const_cast<Mat&>(m));
        }


        static Mat identity() noexcept {
            auto mat = Mat();
            for(int i = 0; i < Order; i++) {
                mat[i][i] = 1.0f;
            }
            return mat;
        }


        float determinant() const noexcept {
            return determinant(*this);
        }


        void invert() noexcept {
            asMat() = adjugate() / determinant();
        }

        Mat inverted() const noexcept {
            auto mat = asMat();
            mat.invert();
            return mat;
        }


        void transpose() noexcept {
            asMat() = transposed();
        }

        Mat transposed() const noexcept {
            auto mat = Mat();
            for(int i = 0; i < Order; i++) {
                for(int j = 0; j < Order; j++) {
                    mat[i][j] = (*this)[j][i];
                }
            }
            return mat;
        }


        Mat operator-() const noexcept {
            return Mat() - asMat();
        }

        Mat operator+() const noexcept {
            return asMat();
        }


        const Vec& operator[](int i) const noexcept {
            return const_cast<Mat&>(asMat())[i];
        }

        Vec& operator[](int i) noexcept {
            return *std::next(begin(asMat()), i);
        }


    private:
        const Mat& asMat() const noexcept {
            return static_cast<const Mat&>(*this);
        }

        Mat& asMat() noexcept {
            return static_cast<Mat&>(*this);
        }


        Mat& combine(const Mat& rhs, auto f) noexcept {
            for(int i = 0; i < Order; i++) {
                for(int j = 0; j < Order; j++) {
                    f((*this)[i][j], rhs[i][j]);
                }
            }
            return asMat();
        }

        Mat& transform(auto f) noexcept {
            for(int i = 0; i < Order; i++) {
                for(int j = 0; j < Order; j++) {
                    f((*this)[i][j]);
                }
            }
            return asMat();
        }


        Mat adjugate() const noexcept {
            auto destMat = Mat();
            for(int i = 0; i < Order; i++) {
                for(int j = 0; j < Order; j++) {
                    destMat[i][j] = cofactor(*this, j, i);
                }
            }
            return destMat;
        }


        // trivial submatrix definition to simplify calculations
        template <typename M>
        struct SubMat {
            static inline constexpr int Order = M::Order - 1;

            SubMat(const M& mat, int skipRow, int skipCol) noexcept {
                for(int srcRow = 0, destRow = 0; srcRow < M::Order; srcRow++) {
                    // skip the row to exclude from the resulting submatrix
                    if(srcRow == skipRow) {
                        continue;
                    }

                    for(int srcCol = 0, destCol = 0; srcCol < M::Order; srcCol++) {
                        // skip the column to exclude from the resulting submatrix
                        if(srcCol == skipCol) {
                            continue;
                        }
                        data[destRow][destCol] = mat[srcRow][srcCol];
                        destCol++;
                    }
                    destRow++;
                }
            }

            const auto& operator[](int i) const noexcept {
                return data[i];
            }

            std::array<std::array<float, Order>, Order> data;
        };


        static float minor(const auto& mat, int row, int col) noexcept {
            // compute the determinant of the submatrix of the original matrix with the row and column removed
            return determinant(SubMat(mat, row, col));
        }

        static float cofactor(const auto& mat, int row, int col) noexcept {
            return (((row + col) % 2 == 0) ? 1 : -1) * minor(mat, row, col);
        }

        template <typename M>
        static float determinant(const M& mat) noexcept {
            float res = 0.0f;
            if constexpr(M::Order != 1) {
                // use cofactor expansion to find the determinant of the matrix
                for(int j = 0; j < M::Order; j++) {
                    res += mat[0][j] * cofactor(mat, 0, j);
                }
            } else {
                // if the matrix has order 1, the determinant is the only element of the matrix
                res += mat[0][0];
            }
            return res;
        }
    };

}
