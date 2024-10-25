#include "OverloadSet.hpp"
#include "util.hpp"

#include <sovren/math/Mat2x2.hpp>
#include <sovren/math/Mat3x3.hpp>
#include <sovren/math/Mat4x4.hpp>
#include <sovren/math/Vec2.hpp>
#include <sovren/math/Vec3.hpp>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

#include <concepts>
#include <type_traits>


#define TYPED_MATRIX_SCENARIO(testName, ...) \
    TEMPLATE_TEST_CASE_METHOD(MatrixTestBase, "Scenario: " testName, "[math][matrix]", __VA_ARGS__)

#define MATRIX_SCENARIO(testName) TYPED_MATRIX_SCENARIO(testName, Mat2x2, Mat3x3, Mat4x4)


using namespace sovren;


template <typename Mat>
class MatrixTestBase {
protected:
    const Mat mat1 = fillMatrix<Mat>(1, 1);
    const Mat mat2 = fillMatrix<Mat>(2, 2);
    Mat empty;
};


MATRIX_SCENARIO("iterating over matrix elements") {
    GIVEN("a matrix") {
        THEN("the iteration visits every element in the matrix") {
            const auto matSize = static_cast<float>(TestType::order() * TestType::order());
            float nextVal = 0;

            for(const auto& row : this->mat1) {
                for(const auto& col : row) {
                    nextVal++;
                    REQUIRE(col == nextVal);
                }
            }

            REQUIRE(nextVal == matSize);
        }
    }
}


MATRIX_SCENARIO("iterating over and mutating matrix elements") {
    GIVEN("a matrix") {
        THEN("the iteration visits and correctly updates every element in the matrix") {
            const auto matSize = static_cast<float>(TestType::order() * TestType::order());
            float nextVal = 0;

            for(auto& row : this->empty) {
                for(auto& col : row) {
                    nextVal++;
                    col = nextVal;
                }
            }

            REQUIRE(nextVal == matSize);
            REQUIRE(this->empty == this->mat1);
        }
    }
}


MATRIX_SCENARIO("comparing matrices for equality") {
    GIVEN("two matrices") {
        WHEN("the elements of the first matrix are equal to the elements of the second matrix") {
            THEN("compare the matrices to be equal") {
                REQUIRE(this->mat1 == this->mat1);
            }
        }

        WHEN("the elements of the first matrix are equal to the elements of the second matrix") {
            THEN("compare the matrices to be not equal") {
                REQUIRE(this->mat1 != this->mat2);
            }
        }
    }
}


MATRIX_SCENARIO("adding matrices") {
    GIVEN("two matrices") {
        THEN("add the corresponding elements of the two matrices") {
            REQUIRE((this->mat1 + this->mat1) == this->mat2);
        }
    }
}


MATRIX_SCENARIO("subtracting matrices") {
    GIVEN("two matrices") {
        THEN("subtract the corresponding elements of the two matrices") {
            REQUIRE((this->mat2 - this->mat1) == this->mat1);
        }
    }
}


MATRIX_SCENARIO("adding a matrix with a scalar") {
    GIVEN("a matrix and a number") {
        const float num = 5;
        THEN("increase each element in the matrix by the number") {
            const auto mat3 = fillMatrix<TestType>(6, 1);
            REQUIRE((this->mat1 + num) == mat3);
        }
    }
}


MATRIX_SCENARIO("subtracting a scalar from a matrix") {
    GIVEN("a matrix and a number") {
        const float num = 2;
        THEN("decrease each element in the matrix by the number") {
            const auto mat3 = fillMatrix<TestType>(0, 2);
            REQUIRE((this->mat2 - num) == mat3);
        }
    }
}


MATRIX_SCENARIO("multiplying a matrix by a scalar") {
    GIVEN("a matrix and a number") {
        const float num = 2;
        THEN("scale each element in the matrix by the number") {
            REQUIRE((this->mat1 * num) == this->mat2);
        }
    }
}


MATRIX_SCENARIO("dividing a matrix by a scalar") {
    GIVEN("a matrix and a number") {
        const float num = 2;
        THEN("scale each element in the matrix by the reciprocal of the number") {
            REQUIRE((this->mat2 / num) == this->mat1);
        }
    }
}


MATRIX_SCENARIO("multiplying matrices") {
    GIVEN("two matrices") {
        THEN("perform matrix-by-matrix multiplication of the matrices") {
            const auto makeExpectedMatrix = overload(
                []<std::same_as<Mat2x2> Mat>() -> Mat {
                    return {
                        { 14, 20 },
                        { 30, 44 }
                    };
                },

                []<std::same_as<Mat3x3> Mat>() -> Mat {
                    return {
                        {  60,  72,  84 },
                        { 132, 162, 192 },
                        { 204, 252, 300 }
                    };
                },

                []<std::same_as<Mat4x4> Mat>() -> Mat {
                    return {
                        { 180, 200,  220,  240 },
                        { 404, 456,  508,  560 },
                        { 628, 712,  796,  880 },
                        { 852, 968, 1084, 1200 }
                    };
                }
            );
            const auto expected = makeExpectedMatrix.template operator()<TestType>();

            REQUIRE((this->mat1 * this->mat2) == expected);
        }
    }
}


MATRIX_SCENARIO("finding the transpose of a matrix") {
    GIVEN("a matrix") {
        THEN("replace the columns of the matrix with its rows") {
            const auto makeExpectedMatrix = overload(
                []<std::same_as<Mat2x2> Mat>() -> Mat {
                    return {
                        { 1, 3 },
                        { 2, 4 }
                    };
                },

                []<std::same_as<Mat3x3> Mat>() -> Mat {
                    return {
                        { 1, 4, 7 },
                        { 2, 5, 8 },
                        { 3, 6, 9 }
                    };
                },

                []<std::same_as<Mat4x4> Mat>() -> Mat {
                    return {
                        { 1, 5,  9, 13 },
                        { 2, 6, 10, 14 },
                        { 3, 7, 11, 15 },
                        { 4, 8, 12, 16 }
                    };
                }
            );
            const auto expected = makeExpectedMatrix.template operator()<TestType>();

            REQUIRE(transpose(this->mat1) == expected);
        }
    }
}


MATRIX_SCENARIO("finding the determinant of a matrix") {
    GIVEN("a matrix") {
        const auto makeSampleMatrix = overload(
            // for finding determinant of 2x2 matrices a simple matrix will do
            [this]<std::same_as<Mat2x2>>() -> const auto& { return this->mat1; },

            // for matrices of other sizes, use ad-hoc matrices to avoid the determinant being 0
            []<std::same_as<Mat3x3> Mat>() -> Mat {
                return {
                    { 5, 2, 3 },
                    { 1, 4, 2 },
                    { 9, 2, 3 }
                };
            },

            []<std::same_as<Mat4x4> Mat>() -> Mat {
                return {
                    { 1, 2, 1, 3 },
                    { 5, 3, 2, 7 },
                    { 1, 3, 8, 9 },
                    { 8, 9, 9, 2 }
                };
            }
        );
        const auto sampleMatrix = makeSampleMatrix.template operator()<TestType>();

        THEN("calculate the determinant of the matrix using the Laplace expansion") {
            // calculate the determinant by hand; use small integers since we only need to check the algorithm itself
            // and can ignore precision problems that may arise
            const auto computeDeterminant = overload(
                []<std::same_as<Mat2x2>>() { return 1 * 4 - 2 * 3; },
                []<std::same_as<Mat3x3>>() { return 5 * (4 * 3 - 2 * 2) - 2 * (1 * 3 - 2 * 9) + 3 * (1 * 2 - 4 * 9); },
                []<std::same_as<Mat4x4>>() {
                    return 1 * (3 * (8 * 2 - 9 * 9) - 2 * (3 * 2 - 9 * 9) + 7 * (3 * 9 - 8 * 9))
                         - 2 * (5 * (8 * 2 - 9 * 9) - 2 * (1 * 2 - 9 * 8) + 7 * (1 * 9 - 8 * 8))
                         + 1 * (5 * (3 * 2 - 9 * 9) - 3 * (1 * 2 - 9 * 8) + 7 * (1 * 9 - 3 * 8))
                         - 3 * (5 * (3 * 9 - 8 * 9) - 3 * (1 * 9 - 8 * 8) + 2 * (1 * 9 - 3 * 8));
                }
            );
            const auto expected = computeDeterminant.template operator()<TestType>();

            REQUIRE(determinant(sampleMatrix) == expected);
        }
    }
}


MATRIX_SCENARIO("finding the inverse of a matrix") {
    GIVEN("a matrix") {
        const auto makeSampleMatrix = overload(
            // for finding inverses of 2x2 matrices a simple matrix will do
            [this]<std::same_as<Mat2x2>>() -> const auto& { return this->mat1; },

            // for matrices of other sizes, use ad-hoc matrices to avoid the determinant being 0
            []<std::same_as<Mat3x3> Mat>() -> Mat {
                return {
                    { 3, 2, 1 },
                    { 2, 3, 1 },
                    { 1, 3, 1 }
                };
            },

            []<std::same_as<Mat4x4> Mat>() -> Mat {
                return {
                    { -1,  1,  1, -1 },
                    {  1,  1, -1, -1 },
                    {  1, -1,  1,  1 },
                    { -1,  1, -1,  1 }
                };
            }
        );
        const auto sampleMatrix = makeSampleMatrix.template operator()<TestType>();

        THEN("find a matrix such that, when multiplied by the original matrix, gives the identity matrix") {
            REQUIRE((sampleMatrix * invert(sampleMatrix)) == TestType::identity());
        }
    }
}


TYPED_MATRIX_SCENARIO("transforming a coordinate vector by a matrix", Mat3x3, Mat4x4) {
    GIVEN("a vector and a matrix") {
        using VecType = std::conditional_t<std::same_as<TestType, Mat3x3>, Vec2, Vec3>;
        const auto sampleVector = fillVector<VecType>(1, 1);

        THEN("perform vector-by-matrix multiplication, ignoring the last column of the matrix") {
            const auto makeExpectedVector = overload(
                []<std::same_as<Mat3x3>>() { return Vec2(1 * 1 + 2 * 4 + 7, 1 * 2 + 2 * 5 + 8); },
                []<std::same_as<Mat4x4>>() {
                    return Vec3(1 * 1 + 2 * 5 + 3 * 9 + 13, 1 * 2 + 2 * 6 + 3 * 10 + 14, 1 * 3 + 2 * 7 + 3 * 11 + 15);
                }
            );
            const auto expected = makeExpectedVector.template operator()<TestType>();

            REQUIRE(this->mat1.transformCoord(sampleVector) == expected);
        }
    }
}


TYPED_MATRIX_SCENARIO("transforming a normal vector by a matrix", Mat3x3, Mat4x4) {
    GIVEN("a vector and a matrix") {
        using VecType = std::conditional_t<std::same_as<TestType, Mat3x3>, Vec2, Vec3>;
        const auto sampleVector = fillVector<VecType>(1, 1);

        THEN("perform vector-by-matrix multiplication, ignoring the last column and row of the matrix") {
            const auto makeExpectedVector = overload(
                []<std::same_as<Mat3x3>>() { return Vec2(1 * 1 + 2 * 4, 1 * 2 + 2 * 5); },
                []<std::same_as<Mat4x4>>() {
                    return Vec3(1 * 1 + 2 * 5 + 3 * 9, 1 * 2 + 2 * 6 + 3 * 10, 1 * 3 + 2 * 7 + 3 * 11);
                }
            );
            const auto expected = makeExpectedVector.template operator()<TestType>();

            REQUIRE(this->mat1.transformNormal(sampleVector) == expected);
        }
    }
}
