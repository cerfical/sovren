#include "sovren/math/Mat2x2.hpp"
#include "sovren/math/Mat3x3.hpp"
#include "sovren/math/Mat4x4.hpp"
#include "sovren/math/Vec2.hpp"
#include "sovren/math/Vec3.hpp"

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>


#define TYPED_MATRIX_TEST(testName, ...) \
    TEMPLATE_TEST_CASE(testName, "[math][matrix]", __VA_ARGS__)

#define MATRIX_TEST(testName) \
    TYPED_MATRIX_TEST(testName, Mat2x2, Mat3x3, Mat4x4)


using namespace sovren;


MATRIX_TEST("iterating over matrix elements using a range-for") {
    auto nextVal = 0.0f;

    const auto mat1 = fillMatrix<TestType>(1, 1);
    SECTION("reading elements") {
        for(const auto& row : mat1) {
            for(const auto& col : row) {
                nextVal++;
                REQUIRE(col == nextVal);
            }
        }
        REQUIRE(nextVal == TestType::order() * TestType::order());
    }

    auto mat2 = fillMatrix<TestType>(0, 0);
    SECTION("writing elements") {
        for(auto& row : mat2) {
            for(auto& col : row) {
                nextVal++;
                col = nextVal;
            }
        }
        REQUIRE(nextVal == TestType::order() * TestType::order());
        REQUIRE(mat2 == mat1);
    }
}


MATRIX_TEST("matrices can be compared for equality") {
    const auto mat1 = fillMatrix<TestType>(1, 1);
    const auto mat2 = fillMatrix<TestType>(2, 2);

    SECTION("equal matrices") {
        REQUIRE(mat1 == mat1);
    }

    SECTION("not equal matrices") {
        REQUIRE(mat1 != mat2);
    }
}


MATRIX_TEST("basic arithmetic operations on matrices") {
    const auto mat1 = fillMatrix<TestType>(1, 1);
    const auto mat2 = fillMatrix<TestType>(2, 2);

    SECTION("adding matrices") {
        REQUIRE((mat1 + mat1) == mat2);
    }

    SECTION("subtracting matrices") {
        REQUIRE((mat2 - mat1) == mat1);
    }
}


MATRIX_TEST("basic arithmetic operations on matrices and scalars") {
    const auto mat1 = fillMatrix<TestType>(1, 1);
    const auto num = 5.0f;

    const auto mat2 = fillMatrix<TestType>(6, 1);
    SECTION("adding a scalar to a matrix") {
        REQUIRE((mat1 + num) == mat2);
    }

    SECTION("subtracting a scalar from a matrix") {
        REQUIRE((mat2 - num) == mat1);
    }


    const auto mat3 = fillMatrix<TestType>(5, 5);
    SECTION("multiplying a matrix by a scalar") {
        REQUIRE((mat1 * num) == mat3);
    }

    SECTION("dividing a matrix by a scalar") {
        REQUIRE((mat3 / num) == mat1);
    }
}


MATRIX_TEST("finding the transpose of a matrix") {
    const auto mat = fillMatrix<TestType>(1, 1);
    auto expected = TestType();
    for(int i = 0; i < TestType::order(); i++) {
        for(int j = 0; j < TestType::order(); j++) {
            expected[j][i] = mat[i][j];
        }
    }
    REQUIRE(transpose(mat) == expected);
}


TYPED_MATRIX_TEST("multiplying matrices", Mat2x2) {
    const auto mat1 = fillMatrix<Mat2x2>(1, 1);
    const auto mat2 = fillMatrix<Mat2x2>(2, 2);

    const auto expected = Mat2x2(
        { 14, 20 },
        { 30, 44 }
    );
    REQUIRE((mat1 * mat2) == expected);
}

TYPED_MATRIX_TEST("multiplying matrices", Mat3x3) {
    const auto mat1 = fillMatrix<Mat3x3>(1, 1);
    const auto mat2 = fillMatrix<Mat3x3>(2, 2);

    const auto expected = Mat3x3(
        { 60, 72, 84 },
        { 132, 162, 192 },
        { 204, 252, 300 }
    );
    REQUIRE((mat1 * mat2) == expected);
}

TYPED_MATRIX_TEST("multiplying matrices", Mat4x4) {
    const auto mat1 = fillMatrix<Mat4x4>(1, 1);
    const auto mat2 = fillMatrix<Mat4x4>(2, 2);

    const auto expected = Mat4x4(
        { 180, 200, 220, 240 },
        { 404, 456, 508, 560 },
        { 628, 712, 796, 880 },
        { 852, 968, 1084, 1200 }
    );
    REQUIRE((mat1 * mat2) == expected);
}


TYPED_MATRIX_TEST("finding the determinant of a matrix", Mat2x2) {
    const auto mat = fillMatrix<Mat2x2>(1, 1);
    const auto expected = (1 * 4) - (2 * 3);
    REQUIRE(determinant(mat) == expected);
}

TYPED_MATRIX_TEST("finding the determinant of a matrix", Mat3x3) {
    const auto mat = Mat3x3(
        { 5, 2, 3 },
        { 1, 4, 2 },
        { 9, 2, 3 }
    );
    const auto expected = (5 * (4 * 3 - 2 * 2)) - (2 * (1 * 3 - 2 * 9)) + (3 * (1 * 2 - 4 * 9));
    REQUIRE(determinant(mat) == expected);
}

TYPED_MATRIX_TEST("finding the determinant of a matrix", Mat4x4) {
    const auto mat = Mat4x4(
        { 1, 2, 1, 3 },
        { 5, 3, 2, 7 },
        { 1, 3, 8, 9 },
        { 8, 9, 9, 2 }
    );
    const auto expected = (1 * (3 * (8 * 2 - 9 * 9) - 2 * (3 * 2 - 9 * 9) + 7 * (3 * 9 - 8 * 9)))
                        - (2 * (5 * (8 * 2 - 9 * 9) - 2 * (1 * 2 - 9 * 8) + 7 * (1 * 9 - 8 * 8)))
                        + (1 * (5 * (3 * 2 - 9 * 9) - 3 * (1 * 2 - 9 * 8) + 7 * (1 * 9 - 3 * 8)))
                        - (3 * (5 * (3 * 9 - 8 * 9) - 3 * (1 * 9 - 8 * 8) + 2 * (1 * 9 - 3 * 8)));
    REQUIRE(determinant(mat) == expected);
}


TYPED_MATRIX_TEST("finding the inverse of a matrix", Mat2x2) {
    const auto mat = fillMatrix<Mat2x2>(1, 1);
    REQUIRE((mat * invert(mat)) == Mat2x2::identity());
}

TYPED_MATRIX_TEST("finding the inverse of a matrix", Mat3x3) {
    const auto mat = Mat3x3(
        { 3, 2, 1 },
        { 2, 3, 1 },
        { 1, 3, 1 }
    );
    REQUIRE((mat * invert(mat)) == Mat3x3::identity());
}

TYPED_MATRIX_TEST("finding the inverse of a matrix", Mat4x4) {
    const auto mat = Mat4x4(
        { -1, 1, 1, -1 },
        { 1, 1, -1, -1 },
        { 1, -1, 1, 1 },
        { -1, 1, -1, 1 }
    );
    REQUIRE((mat * invert(mat)) == Mat4x4::identity());
}


TYPED_MATRIX_TEST("transforming a coordinate vector by a matrix", Mat3x3) {
    const auto mat = fillMatrix<Mat3x3>(1, 1);
    const auto coord = fillVector<Vec2>(1, 1);
    const auto expected = Vec2((1 * 1) + (2 * 4) + 7, (1 * 2) + (2 * 5) + 8);

    REQUIRE(mat.transformCoord(coord) == expected);
}

TYPED_MATRIX_TEST("transforming a coordinate vector by a matrix", Mat4x4) {
    const auto mat = fillMatrix<Mat4x4>(1, 1);
    const auto coord = fillVector<Vec3>(1, 1);
    const auto expected = Vec3(
        (1 * 1) + (2 * 5) + (3 * 9) + 13,
        (1 * 2) + (2 * 6) + (3 * 10) + 14,
        (1 * 3) + (2 * 7) + (3 * 11) + 15
    );

    REQUIRE(mat.transformCoord(coord) == expected);
}


TYPED_MATRIX_TEST("transforming a normal vector by a matrix", Mat3x3) {
    const auto mat = fillMatrix<Mat3x3>(1, 1);
    const auto norm = fillVector<Vec2>(1, 1);
    const auto expected = Vec2((1 * 1) + (2 * 4), (1 * 2) + (2 * 5));

    REQUIRE(mat.transformNormal(norm) == expected);
}

TYPED_MATRIX_TEST("transforming a normal vector by a matrix", Mat4x4) {
    const auto mat = fillMatrix<Mat4x4>(1, 1);
    const auto norm = fillVector<Vec3>(1, 1);
    const auto expected = Vec3(
        (1 * 1) + (2 * 5) + (3 * 9),
        (1 * 2) + (2 * 6) + (3 * 10),
        (1 * 3) + (2 * 7) + (3 * 11)
    );

    REQUIRE(mat.transformNormal(norm) == expected);
}
