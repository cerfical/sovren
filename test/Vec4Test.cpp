#include <gtest/gtest.h>

#include <reni/math.hpp>
#include <reni/print.hpp>

using namespace reni;

TEST(Vec4, IterationOverComponents) {
	const auto v = Vec4(1.0, 2.0, 3.0, 4.0);
	float nextVal = 0.0;

	for(const auto& val : v) {
		EXPECT_EQ(val, (nextVal += 1.0));
	}
	EXPECT_EQ(nextVal, 4.0);
}

TEST(Vec4, MutatingIterationOverComponents) {
	Vec4 v;
	float nextVal = 0.0;

	for(auto& val : v) {
		val = (nextVal += 1.0);
	}
	
	EXPECT_EQ(nextVal, 4.0);
	EXPECT_EQ(v, Vec4(1.0, 2.0, 3.0, 4.0));
}

TEST(Vec4, EqualityComparison) {
	EXPECT_EQ(Vec4(1.0, 2.0, 3.0, 4.0), Vec4(1.0, 2.0, 3.0, 4.0));
	EXPECT_NE(Vec4(1.0, 2.0, 3.0, 4.0), Vec4(4.0, 3.0, 2.0, 1.0));
}

TEST(Vec4, GreaterThanComparison) {
	EXPECT_GT(Vec4(4.0, 3.0, 2.0, 1.0), Vec4(1.0, 2.0, 3.0, 4.0));
	EXPECT_LE(Vec4(1.0, 2.0, 3.0, 4.0), Vec4(4.0, 3.0, 2.0, 1.0));
}

TEST(Vec4, LessThanComparison) {
	EXPECT_LT(Vec4(1.0, 2.0, 3.0, 4.0), Vec4(4.0, 3.0, 2.0, 1.0));
	EXPECT_GE(Vec4(4.0, 3.0, 2.0, 1.0), Vec4(1.0, 2.0, 3.0, 4.0));
}

TEST(Vec4, Addition) {
	ASSERT_EQ(Vec4(1.0, 2.0, 3.0, 4.0) + Vec4(3.0, 4.0, 5.0, 6.0), Vec4(4.0, 6.0, 8.0, 10.0));
}

TEST(Vec4, Subtraction) {
	ASSERT_EQ(Vec4(3.0, 2.0, 1.0, 0.0) - Vec4(1.0, 2.0, 3.0, 4.0), Vec4(2.0, 0.0, -2.0, -4.0));
}
