#pragma once

#include <concepts>
#include <iterator>
#include <type_traits>

namespace reni::math {

    template <typename Mat, typename Sub, typename Vec>
    struct MatBase {

        friend bool operator==(MatBase, MatBase) noexcept = default;


        friend Mat operator+(Mat lhs, const Mat& rhs) noexcept { return lhs += rhs; }
        friend Mat operator-(Mat lhs, const Mat& rhs) noexcept { return lhs -= rhs; }
        friend Mat operator*(Mat lhs, const Mat& rhs) noexcept { return lhs *= rhs; }

        friend Mat operator+(Mat lhs, float rhs) noexcept { return lhs += rhs; }
        friend Mat operator-(Mat lhs, float rhs) noexcept { return lhs -= rhs; }
        friend Mat operator*(Mat lhs, float rhs) noexcept { return lhs *= rhs; }
        friend Mat operator/(Mat lhs, float rhs) noexcept { return lhs /= rhs; }


        friend const Vec* begin(const Mat& m) noexcept { return begin(const_cast<Mat&>(m)); }

        friend const Vec* end(const Mat& m) noexcept { return end(const_cast<Mat&>(m)); }


        static Mat identity() noexcept {
            Mat mat;
            for(int i = 0; i < mat.order(); i++) {
                mat[i][i] = 1.0f;
            }
            return mat;
        }


        template <std::invocable<float, float> F>
        float fold(float v, F f) const noexcept {
            for(const auto& row : asMat()) {
                for(const auto& col : row) {
                    v = f(v, col);
                }
            }
            return v;
        }


        template <std::invocable<float, float> F>
        Mat combine(const Mat& rhs, F f) const noexcept {
            auto mat = Mat();
            for(int i = 0; i < order(); i++) {
                for(int j = 0; j < order(); j++) {
                    mat[i][j] = f(asMat()[i][j], rhs[i][j]);
                }
            }
            return mat;
        }


        template <std::invocable<float> F>
        Mat transform(F f) const noexcept {
            auto mat = Mat();
            for(int i = 0; i < order(); i++) {
                for(int j = 0; j < order(); j++) {
                    mat[i][j] = f(asMat()[i][j]);
                }
            }
            return mat;
        }


        Mat transpose() const noexcept {
            const auto& srcMat = asMat();
            auto destMat = Mat();

            for(int i = 0; i < srcMat.order(); i++) {
                for(int j = 0; j < srcMat.order(); j++) {
                    destMat[i][j] = srcMat[j][i];
                }
            }

            return destMat;
        }

        Mat inverse() const noexcept { return adjugate() / determinant(); }


        float determinant() const noexcept {
            return asMat()[0].fold(0.0f, [this, j = 0](auto v, auto col) mutable { return v + col * cofactor(0, j++); });
        }

        float sum() const noexcept {
            return fold(0.0f, [](auto l, auto r) { return l + r; });
        }


        Mat& operator+=(const Mat& rhs) noexcept {
            return asMat() = combine(rhs, [](auto l, auto r) { return l + r; });
        }

        Mat& operator-=(const Mat& rhs) noexcept {
            return asMat() = combine(rhs, [](auto l, auto r) { return l - r; });
        }

        Mat& operator*=(const Mat& rhs) noexcept {
            auto& lhs = asMat();
            for(int i = 0; i < lhs.order(); i++) {
                lhs[i] *= rhs;
            }
            return lhs;
        }


        Mat& operator+=(float rhs) noexcept {
            return asMat() = transform([rhs](auto l) { return l + rhs; });
        }

        Mat& operator-=(float rhs) noexcept {
            return asMat() = transform([rhs](auto l) { return l - rhs; });
        }

        Mat& operator*=(float rhs) noexcept {
            return asMat() = transform([rhs](auto l) { return l * rhs; });
        }

        Mat& operator/=(float rhs) noexcept {
            return asMat() = transform([rhs](auto l) { return l / rhs; });
        }


        const Vec& operator[](int i) const noexcept { return const_cast<Mat&>(asMat())[i]; }

        Vec& operator[](int i) noexcept { return *std::next(begin(asMat()), i); }


        int order() const noexcept { return static_cast<int>(end(asMat()) - begin(asMat())); }

        int size() const noexcept { return order() * order(); }


    private:
        const Mat& asMat() const noexcept { return static_cast<const Mat&>(*this); }

        Mat& asMat() noexcept { return static_cast<Mat&>(*this); }


        Mat adjugate() const noexcept {
            const auto& srcMat = asMat();
            auto destMat = Mat();

            for(int i = 0; i < srcMat.order(); i++) {
                for(int j = 0; j < srcMat.order(); j++) {
                    destMat[i][j] = cofactor(j, i);
                }
            }
            return destMat;
        }

        float cofactor(int row, int col) const noexcept { return (((row + col) % 2 == 0) ? 1 : -1) * minor(row, col); }

        float minor(int row, int col) const noexcept { return submatrix(row, col).determinant(); }


        // trivial singleton matrix definition to simplify submatrix computation
        struct Vec1 {
            float& operator[](int) noexcept { return x; }

            float x = {};
        };

        struct Mat1x1 {
            Vec1& operator[](int) noexcept { return row; }

            float determinant() const noexcept { return row.x; }

            Vec1 row;
        };


        using SubmatrixType = std::conditional_t<std::same_as<Sub, float>, Mat1x1, Sub>;

        SubmatrixType submatrix(int row, int col) const noexcept {
            SubmatrixType destMat;
            for(int srcRow = 0, destRow = 0; srcRow < order(); srcRow++) {
                // skip the row to exclude from the resulting submatrix
                if(srcRow == row) {
                    continue;
                }

                for(int srcCol = 0, destCol = 0; srcCol < order(); srcCol++) {
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
