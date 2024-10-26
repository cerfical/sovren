#include "sovren/math/Mat2x2.hpp"
#include "sovren/math/Mat3x3.hpp"
#include "sovren/math/Mat4x4.hpp"
#include "sovren/math/Vec2.hpp"
#include "sovren/math/Vec3.hpp"
#include "sovren/math/Vec4.hpp"

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>


#define TYPED_VECTOR_TEST(testName, ...) \
    TEMPLATE_TEST_CASE(testName, "[math][vector]", __VA_ARGS__)

#define VECTOR_TEST(testName) \
    TYPED_VECTOR_TEST(testName, Vec2, Vec3, Vec4)


using namespace Catch::Matchers;
using namespace sovren;


VECTOR_TEST("iterating over vector components using a range-for") {
    auto nextVal = 0.0f;

    const auto vec1 = fillVector<TestType>(1, 1);
    SECTION("reading components") {
        for(const auto& col : vec1) {
            nextVal++;
            REQUIRE(col == nextVal);
        }
        REQUIRE(nextVal == TestType::order());
    }

    auto vec2 = fillVector<TestType>(0, 0);
    SECTION("writing components") {
        for(auto& col : vec2) {
            nextVal++;
            col = nextVal;
        }
        REQUIRE(nextVal == TestType::order());
        REQUIRE(vec2 == vec1);
    }
}


VECTOR_TEST("vectors can be compared") {
    const auto smaller = fillVector<TestType>(1, 1);
    const auto greater = fillVector<TestType>(2, 2);

    SECTION("equal vectors") {
        REQUIRE(smaller == smaller);
    }

    SECTION("not equal vectors") {
        REQUIRE(smaller != greater);
    }

    SECTION("one vector is greater than the other") {
        REQUIRE(greater > smaller);
    }

    SECTION("one vector is smaller than the other") {
        REQUIRE(smaller < greater);
    }
}


VECTOR_TEST("basic arithmetic operations on vectors") {
    const auto vec1 = fillVector<TestType>(1, 1);
    SECTION("negating a vector") {
        const auto expected = fillVector<TestType>(-1, -1);
        REQUIRE(-vec1 == expected);
    }

    const auto vec2 = fillVector<TestType>(2, 2);
    SECTION("adding vectors") {
        REQUIRE((vec1 + vec1) == vec2);
    }

    SECTION("subtracting vectors") {
        REQUIRE((vec2 - vec1) == vec1);
    }

    const auto vec3 = fillVector<TestType>(2, 0);
    SECTION("multiplying vectors") {
        REQUIRE((vec1 * vec3) == vec2);
    }

    SECTION("dividing vectors") {
        REQUIRE((vec2 / vec3) == vec1);
    }
}


VECTOR_TEST("basic arithmetic operations on vectors and scalars") {
    const auto vec1 = fillVector<TestType>(1, 1);
    const auto num = 5.0f;

    const auto vec2 = fillVector<TestType>(6, 1);
    SECTION("adding a scalar to a vector") {
        REQUIRE((vec1 + num) == vec2);
    }

    SECTION("subtracting a scalar from a vector") {
        REQUIRE((vec2 - num) == vec1);
    }

    const auto vec3 = fillVector<TestType>(5, 5);
    SECTION("multiplying a vector by a scalar") {
        REQUIRE((vec1 * num) == vec3);
    }

    SECTION("dividing a vector by a scalar") {
        REQUIRE((vec3 / num) == vec1);
    }
}


VECTOR_TEST("normalizing a vector") {
    const auto vec = fillVector<TestType>(1, 1);
    const auto normalized = normalize(vec);
    const auto scale = normalized / vec;

    // verify that the normalized vector has not changed the direction
    for(int i = 1; i < TestType::order(); i++) {
        // the original and normalized vector components must be positive multiples of each other
        CHECK(scale[i] > 0);

        // normalized components must be scaled with the same factor
        CHECK_THAT(scale[i], WithinRel(scale[i - 1]));
    }

    // verify that the normalized vector is a unit vector, that is, it has length 1
    REQUIRE_THAT(len(normalized), WithinRel(1.0f));
}


VECTOR_TEST("finding the dot product of vectors") {
    const auto vec1 = fillVector<TestType>(1, 1);
    const auto vec2 = fillVector<TestType>(2, 2);
    REQUIRE(dot(vec1, vec2) == sum(vec1 * vec2));
}


VECTOR_TEST("finding the length of a vector") {
    const auto vec = fillVector<TestType>(1, 1);
    REQUIRE(len(vec) == std::sqrt(dot(vec, vec)));
}


TYPED_VECTOR_TEST("multiplying a vector by a matrix", Vec2) {
    const auto mat = Mat2x2(
        { 2, 0 },
        { 0, 2 }
    );
    REQUIRE((Vec2(1, 2) * mat) == Vec2(2, 4));
}

TYPED_VECTOR_TEST("multiplying a vector by a matrix", Vec3) {
    const auto mat = Mat3x3(
        { 2, 0, 0 },
        { 0, 2, 0 },
        { 0, 0, 2 }
    );
    REQUIRE((Vec3(1, 2, 3) * mat) == Vec3(2, 4, 6));
}

TYPED_VECTOR_TEST("multiplying a vector by a matrix", Vec4) {
    const auto mat = Mat4x4(
        { 2, 0, 0, 0 },
        { 0, 2, 0, 0 },
        { 0, 0, 2, 0 },
        { 0, 0, 0, 2 }
    );
    REQUIRE((Vec4(1, 2, 3, 4) * mat) == Vec4(2, 4, 6, 8));
}


TYPED_VECTOR_TEST("finding the cross product of vectors", Vec3) {
    const auto expected = Vec3((2 * 1) - (3 * 2), (3 * 3) - (1 * 1), (1 * 2) - (2 * 3));
    REQUIRE(cross(Vec3(1, 2, 3), Vec3(3, 2, 1)) == expected);
}
