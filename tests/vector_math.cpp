#include "OverloadSet.hpp"

#include <reni/math/Mat2x2.hpp>
#include <reni/math/Mat3x3.hpp>
#include <reni/math/Mat4x4.hpp>
#include <reni/math/Vec2.hpp>
#include <reni/math/Vec3.hpp>
#include <reni/math/Vec4.hpp>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <concepts>


#define TYPED_VECTOR_SCENARIO(testName, ...) \
    TEMPLATE_TEST_CASE_METHOD(VectorTestBase, "Scenario: " testName, "[math][vector]", __VA_ARGS__)

#define VECTOR_SCENARIO(testName) TYPED_VECTOR_SCENARIO(testName, Vec2, Vec3, Vec4)


using namespace reni;
using namespace Catch::Matchers;


template <typename Vec>
class VectorTestBase {
protected:
    Vec fillVector(float start, float step) {
        Vec vec;
        for(float nextVal = start; auto& col : vec) {
            col = nextVal;
            nextVal += step;
        }
        return vec;
    }

    const Vec vec1 = fillVector(1, 1);
    const Vec vec2 = fillVector(2, 2);
    Vec empty;
};


VECTOR_SCENARIO("iterating over vector components") {
    GIVEN("a vector") {
        THEN("the iteration visits every component of the vector") {
            float nextVal = 0;

            for(const auto& col : this->vec1) {
                nextVal++;
                REQUIRE(col == nextVal);
            }

            REQUIRE(nextVal == this->vec1.size());
        }
    }
}


VECTOR_SCENARIO("iterating over and mutating vector components") {
    GIVEN("a vector") {
        THEN("the iteration visits and correctly updates every component of the vector") {
            float nextVal = 0;

            for(auto& col : this->empty) {
                nextVal++;
                col = nextVal;
            }

            REQUIRE(nextVal == this->empty.size());
            REQUIRE(this->empty == this->vec1);
        }
    }
}


VECTOR_SCENARIO("comparing vectors") {
    GIVEN("two vectors") {
        WHEN("the components of the first vector are equal to the components of the second vector") {
            THEN("compare the vectors to be equal") {
                REQUIRE(this->vec1 == this->vec1);
            }
        }

        WHEN("the components of the first vector are not equal to the components of the second vector") {
            THEN("compare the vectors to be not equal") {
                REQUIRE(this->vec1 != this->vec2);
            }
        }

        WHEN("the components of the first vector lexicographically greater than the components of the second vector") {
            THEN("compare the first vector to be greater than the second vector") {
                REQUIRE(this->vec2 > this->vec1);
            }
        }

        WHEN("the components of the first vector lexicographically less than the components of the second vector") {
            THEN("compare the first vector to be less than the second vector") {
                REQUIRE(this->vec1 < this->vec2);
            }
        }
    }
}


VECTOR_SCENARIO("adding vectors") {
    GIVEN("two vectors") {
        THEN("add the corresponding components of the vectors") {
            REQUIRE((this->vec1 + this->vec1) == this->vec2);
        }
    }
}


VECTOR_SCENARIO("subtracting vectors") {
    GIVEN("two vectors") {
        THEN("subtract the corresponding components of the vectors") {
            REQUIRE((this->vec2 - this->vec1) == this->vec1);
        }
    }
}


VECTOR_SCENARIO("multiplying vectors") {
    GIVEN("two vectors") {
        const auto vec3 = this->fillVector(2, 0);
        THEN("multiply the corresponding components of the vectors") {
            REQUIRE((this->vec1 * vec3) == this->vec2);
        }
    }
}


VECTOR_SCENARIO("dividing vectors") {
    GIVEN("two vectors") {
        const auto vec3 = this->fillVector(2, 0);
        THEN("divide the corresponding components of the vectors") {
            REQUIRE((this->vec2 / vec3) == this->vec1);
        }
    }
}


VECTOR_SCENARIO("adding a vector with a scalar") {
    GIVEN("a vector and a number") {
        const float num = 5;
        THEN("increase each component in the vector by the number") {
            const auto vec3 = this->fillVector(6, 1);
            REQUIRE((this->vec1 + num) == vec3);
        }
    }
}


VECTOR_SCENARIO("subtracting a scalar from a vector") {
    GIVEN("a vector and a number") {
        const float num = 5;
        THEN("decrease each component in the vector by the number") {
            const auto vec3 = this->fillVector(-4, 1);
            REQUIRE((this->vec1 - num) == vec3);
        }
    }
}


VECTOR_SCENARIO("multiplying a vector by a scalar") {
    GIVEN("a vector and a number") {
        const float num = 2;
        THEN("scale each component in the vector by the number") {
            REQUIRE((this->vec1 * num) == this->vec2);
        }
    }
}


VECTOR_SCENARIO("dividing a vector by a scalar") {
    GIVEN("a vector and a number") {
        const float num = 2;
        THEN("scale each component in the vector by the reciprocal of the number") {
            REQUIRE((this->vec2 / num) == this->vec1);
        }
    }
}


VECTOR_SCENARIO("multiplying a vector by a matrix") {
    GIVEN("a vector and a matrix") {
        const auto makeSampleMatrix = overload(
            []<std::same_as<Vec2>>() -> Mat2x2 {
                return {
                    { 2, 0 },
                    { 0, 2 }
                };
            },

            []<std::same_as<Vec3>>() -> Mat3x3 {
                return {
                    { 2, 0, 0 },
                    { 0, 2, 0 },
                    { 0, 0, 2 }
                };
            },

            []<std::same_as<Vec4>>() -> Mat4x4 {
                return {
                    { 2, 0, 0, 0 },
                    { 0, 2, 0, 0 },
                    { 0, 0, 2, 0 },
                    { 0, 0, 0, 2 }
                };
            }
        );
        const auto sampleMatrix = makeSampleMatrix.template operator()<TestType>();

        THEN("perform matrix-by-matrix multiplication treating the vector as a one-row matrix") {
            REQUIRE((this->vec1 * sampleMatrix) == this->vec2);
        }
    }
}


VECTOR_SCENARIO("finding the dot product of vectors") {
    GIVEN("two vectors") {
        THEN("multiply the corresponding components of both vectors and sum the result") {
            const auto computeDotProduct = overload(
                []<std::same_as<Vec2>>() { return 1 * 2 + 2 * 4; },
                []<std::same_as<Vec3>>() { return 1 * 2 + 2 * 4 + 3 * 6; },
                []<std::same_as<Vec4>>() { return 1 * 2 + 2 * 4 + 3 * 6 + 4 * 8; }
            );
            const auto expected = computeDotProduct.template operator()<TestType>();

            REQUIRE(this->vec1.dot(this->vec2) == expected);
        }
    }
}


TYPED_VECTOR_SCENARIO("finding the cross product of vectors", Vec3) {
    GIVEN("two vectors") {
        const auto vec3 = Vec3(3, 2, 1);
        THEN("find their cross product") {
            const auto expected = Vec3(2 * 1 - 3 * 2, 3 * 3 - 1 * 1, 1 * 2 - 2 * 3);
            REQUIRE(this->vec1.cross(vec3) == expected);
        }
    }
}


VECTOR_SCENARIO("finding the length of a vector") {
    GIVEN("a vector") {
        const auto makeSampleVector = overload(
            // 3 * 3 + 4 * 4 = 25 = 5 * 5
            []<std::same_as<Vec2> Vec>() -> Vec { return { 3, 4 }; },

            // 8 * 8 + 4 * 4 + 1 * 1 = 81 = 9 * 9
            []<std::same_as<Vec3> Vec>() -> Vec { return { 8, 4, 1 }; },

            // 7 * 7 + 1 * 1 + 7 * 7 + 1 * 1 = 100 = 10 * 10
            []<std::same_as<Vec4> Vec>() -> Vec { return { 7, 1, 7, 1 }; }
        );
        const auto sampleVector = makeSampleVector.template operator()<TestType>();

        THEN("compute the dot product of the vector with itself and take the square root of the result") {
            const auto computeLength = overload(
                []<std::same_as<Vec2>>() { return 5; },
                []<std::same_as<Vec3>>() { return 9; },
                []<std::same_as<Vec4>>() { return 10; }
            );
            const auto expected = computeLength.template operator()<TestType>();

            REQUIRE(sampleVector.len() == expected);
        }
    }
}


VECTOR_SCENARIO("normalizing a vector") {
    GIVEN("a vector") {
        THEN("transform the vector into a unit vector with the same direction") {
            const auto normalized = this->vec1.normalize();
            const auto scale = normalized / this->vec1;

            // verify that the normalized vector has not changed the direction
            for(int i = 1; i < scale.size(); i++) {
                // the original and normalized vector components must be positive multiples of each other
                CHECK(scale[i] > 0.0f);

                // normalized components must be scaled with the same factor
                CHECK_THAT(scale[i], WithinRel(scale[i - 1]));
            }

            // also verify that the normalized vector is a unit vector, that is, it has length 1
            REQUIRE_THAT(normalized.len(), WithinRel(1.0f));
        }
    }
}


VECTOR_SCENARIO("negating a vector") {
    GIVEN("a vector") {
        THEN("negate the components of the vector") {
            const auto expected = this->fillVector(-1, -1);
            REQUIRE(-this->vec1 == expected);
        }
    }
}
