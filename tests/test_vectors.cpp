#include "../include/wheel_util_math.hpp"
#include "gtest/gtest.h"

namespace {
   class VectorTests : public ::testing::Test
   {
      void SetUp()
      {
      }
   };
}


TEST_F(VectorTests, 2DVector)
{
   wheel::vec2<float> v2 {1.0f, 0.1f};
   EXPECT_EQ(1.0f, v2[0]);
   EXPECT_EQ(0.1f, v2[1]);

   v2.x = v2.v;
   EXPECT_EQ(0.1f, v2.x);
}

int main(int argc, char* argv[])
{
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}