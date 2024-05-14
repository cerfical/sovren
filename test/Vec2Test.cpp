#include <gtest/gtest.h>

#include <reni/math.hpp>
#include <reni/print.hpp>

using namespace reni;

TEST(Vec2, IterationOverComponents) {
	const auto v = Vec2(1.0, 2.0);
	float nextVal = 0.0;

	for(const auto& val : v) {
		EXPECT_EQ(val, (nextVal += 1.0));
	}
	EXPECT_EQ(nextVal, 2.0);
}

TEST(Vec2, MutatingIterationOverComponents) {
	Vec2 v;
	float nextVal = 0.0;

	for(auto& val : v) {
		val = (nextVal += 1.0);
	}
	
	EXPECT_EQ(nextVal, 2.0);
	EXPECT_EQ(v, Vec2(1.0, 2.0));
}

TEST(Vec2, EqualityComparison) {
	EXPECT_EQ(Vec2(1.0, 2.0), Vec2(1.0, 2.0));
	EXPECT_NE(Vec2(1.0, 2.0), Vec2(2.0, 1.0));
}

TEST(Vec2, GreaterThanComparison) {
	EXPECT_GT(Vec2(2.0, 1.0), Vec2(1.0, 2.0));
	EXPECT_LE(Vec2(1.0, 2.0), Vec2(2.0, 1.0));
}

TEST(Vec2, LessThanComparison) {
	EXPECT_LT(Vec2(1.0, 2.0), Vec2(2.0, 1.0));
	EXPECT_GE(Vec2(2.0, 1.0), Vec2(1.0, 2.0));
}

TEST(Vec2, Addition) {
	ASSERT_EQ(Vec2(1.0, 2.0) + Vec2(3.0, 4.0), Vec2(4.0, 6.0));
}

TEST(Vec2, Subtraction) {
	ASSERT_EQ(Vec2(3.0, 2.0) - Vec2(1.0, 2.0), Vec2(2.0, 0.0));
}
