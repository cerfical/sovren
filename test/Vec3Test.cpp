#include <gtest/gtest.h>

#include <reni/math.hpp>
#include <reni/print.hpp>

using namespace reni;

TEST(Vec3, IterationOverComponents) {
	const auto v = Vec3(1.0, 2.0, 3.0);
	float nextVal = 0.0;

	for(const auto& val : v) {
		EXPECT_EQ(val, (nextVal += 1.0));
	}
	EXPECT_EQ(nextVal, 3.0);
}

TEST(Vec3, MutatingIterationOverComponents) {
	Vec3 v;
	float nextVal = 0.0;

	for(auto& val : v) {
		val = (nextVal += 1.0);
	}
	
	EXPECT_EQ(nextVal, 3.0);
	EXPECT_EQ(v, Vec3(1.0, 2.0, 3.0));
}

TEST(Vec3, EqualityComparison) {
	EXPECT_EQ(Vec3(1.0, 2.0, 3.0), Vec3(1.0, 2.0, 3.0));
	EXPECT_NE(Vec3(1.0, 2.0, 3.0), Vec3(3.0, 2.0, 1.0));
}

TEST(Vec3, GreaterThanComparison) {
	EXPECT_GT(Vec3(3.0, 2.0, 1.0), Vec3(1.0, 2.0, 3.0));
	EXPECT_LE(Vec3(1.0, 2.0, 3.0), Vec3(3.0, 2.0, 1.0));
}

TEST(Vec3, LessThanComparison) {
	EXPECT_LT(Vec3(1.0, 2.0, 3.0), Vec3(3.0, 2.0, 1.0));
	EXPECT_GE(Vec3(3.0, 2.0, 1.0), Vec3(1.0, 2.0, 3.0));
}

TEST(Vec3, Addition) {
	ASSERT_EQ(Vec3(1.0, 2.0, 3.0) + Vec3(3.0, 4.0, 5.0), Vec3(4.0, 6.0, 8.0));
}

TEST(Vec3, Subtraction) {
	ASSERT_EQ(Vec3(3.0, 2.0, 1.0) - Vec3(1.0, 2.0, 3.0), Vec3(2.0, 0.0, -2.0));
}
