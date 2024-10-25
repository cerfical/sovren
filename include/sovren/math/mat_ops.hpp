#pragma once

#include "Mat.hpp"

#include <array>

namespace sovren {

    template <Mat M>
    auto begin(M& mat) noexcept -> Vec auto* {
        return &mat[0];
    }

    template <Mat M>
    auto begin(const M& mat) noexcept -> const Vec auto* {
        return begin(const_cast<M&>(mat));
    }


    template <Mat M>
    auto end(M& mat) noexcept -> Vec auto* {
        return &mat[M::order() - 1] + 1;
    }

    template <Mat M>
    auto end(const M& mat) noexcept -> const Vec auto* {
        return end(const_cast<M&>(mat));
    }


    template <Mat M>
    auto makeIdentityMatrix() noexcept -> M {
        auto mat = M();
        for(int i = 0; i < M::order(); i++) {
            mat[i][i] = 1;
        }
        return mat;
    }


    template <Mat M>
    auto transform(M& mat, auto f) noexcept -> M& {
        for(int i = 0; i < M::order(); i++) {
            for(int j = 0; j < M::order(); j++) {
                mat[i][j] = f(mat[i][j]);
            }
        }
        return mat;
    }

    template <Mat M>
    auto combine(M& lhs, const M& rhs, auto f) noexcept -> M& {
        for(int i = 0; i < M::order(); i++) {
            for(int j = 0; j < M::order(); j++) {
                lhs[i][j] = f(lhs[i][j], rhs[i][j]);
            }
        }
        return lhs;
    }


    template <Mat M>
    auto adjugate(const M& mat) noexcept -> M {
        auto destMat = M();
        for(int i = 0; i < M::order(); i++) {
            for(int j = 0; j < M::order(); j++) {
                destMat[i][j] = cofactor(mat, j, i);
            }
        }
        return destMat;
    }

    template <Mat M>
    auto minor(const M& mat, int row, int col) noexcept -> float {
        struct SubVec {
            static consteval auto order() noexcept -> int {
                return M::order() - 1;
            }

            auto operator[](int i) const noexcept -> const float& {
                return data[i];
            }

            auto operator[](int i) noexcept -> float& {
                return data[i];
            }

            std::array<float, M::order() - 1> data = { 0 };
        };

        // submatrix to simplify calculations
        struct SubMat {
            static consteval auto order() noexcept -> int {
                return M::order() - 1;
            }

            SubMat(const M& mat, int skipRow, int skipCol) noexcept {
                for(int srcRow = 0, destRow = 0; srcRow < M::order(); srcRow++) {
                    // skip the row to exclude from the resulting submatrix
                    if(srcRow == skipRow) {
                        continue;
                    }

                    for(int srcCol = 0, destCol = 0; srcCol < M::order(); srcCol++) {
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

            auto operator[](int i) const noexcept -> const auto& {
                return data[i];
            }

            auto operator[](int i) noexcept -> auto& {
                return data[i];
            }

            std::array<SubVec, M::order() - 1> data;
        };

        // compute the determinant of the submatrix of the original matrix with the row and column removed
        return determinant(SubMat(mat, row, col));
    }

    template <Mat M>
    auto cofactor(const M& mat, int row, int col) noexcept -> float {
        return (((row + col) % 2 == 0) ? 1 : -1) * minor(mat, row, col);
    }

    template <Mat M>
    auto determinant(const M& mat) noexcept -> float {
        float res = 0;
        if constexpr(M::order() != 1) {
            // use cofactor expansion to find the determinant of the matrix
            for(int j = 0; j < M::order(); j++) {
                res += mat[0][j] * cofactor(mat, 0, j);
            }
        } else {
            // if the matrix has order 1, the determinant is the only element of the matrix
            res += mat[0][0];
        }
        return res;
    }


    template <Mat M>
    auto operator==(const M& lhs, const M& rhs) noexcept -> bool {
        for(int i = 0; i < M::order(); i++) {
            if(lhs[i] != rhs[i]) {
                return false;
            }
        }
        return true;
    }


    template <Mat M>
    auto operator+(M lhs, const M& rhs) noexcept -> M {
        return combine(lhs, rhs, [](auto l, auto r) { return l + r; });
    }

    template <Mat M>
    auto operator-(M lhs, const M& rhs) noexcept -> M {
        return combine(lhs, rhs, [](auto l, auto r) { return l - r; });
    }

    template <Mat M>
    auto operator*(M lhs, const M& rhs) noexcept -> M {
        for(int i = 0; i < M::order(); i++) {
            lhs[i] *= rhs;
        }
        return lhs;
    }


    template <Mat M>
    auto operator+(M lhs, float rhs) noexcept -> M {
        return transform(lhs, [rhs](auto l) { return l + rhs; });
    }

    template <Mat M>
    auto operator-(M lhs, float rhs) noexcept -> M {
        return transform(lhs, [rhs](auto l) { return l - rhs; });
    }

    template <Mat M>
    auto operator*(M lhs, float rhs) noexcept -> M {
        return transform(lhs, [rhs](auto l) { return l * rhs; });
    }

    template <Mat M>
    auto operator/(M lhs, float rhs) noexcept -> M {
        return transform(lhs, [rhs](auto l) { return l / rhs; });
    }


    template <Mat M>
    auto operator+=(M& lhs, const M& rhs) noexcept -> M& {
        return lhs = lhs + rhs;
    }

    template <Mat M>
    auto operator-=(M& lhs, const M& rhs) noexcept -> M& {
        return lhs = lhs - rhs;
    }

    template <Mat M>
    auto operator*=(M& lhs, const M& rhs) noexcept -> M& {
        return lhs = lhs * rhs;
    }


    template <Mat M>
    auto operator+=(M& lhs, float rhs) noexcept -> M& {
        return lhs = lhs + rhs;
    }

    template <Mat M>
    auto operator-=(M& lhs, float rhs) noexcept -> M& {
        return lhs = lhs - rhs;
    }

    template <Mat M>
    auto operator*=(M& lhs, float rhs) noexcept -> M& {
        return lhs = lhs * rhs;
    }

    template <Mat M>
    auto operator/=(M& lhs, float rhs) noexcept -> M& {
        return lhs = lhs / rhs;
    }


    template <Mat M>
    auto invert(const M& mat) noexcept -> M {
        return adjugate(mat) / determinant(mat);
    }


    template <Mat M>
    auto transpose(const M& mat) noexcept -> M {
        auto transposed = M();
        for(int i = 0; i < M::order(); i++) {
            for(int j = 0; j < M::order(); j++) {
                transposed[i][j] = mat[j][i];
            }
        }
        return transposed;
    }


    template <Mat M>
    auto operator-(const M& mat) noexcept -> M {
        return M() - mat;
    }

    template <Mat M>
    auto operator+(const M& mat) noexcept -> M {
        return mat;
    }

}
