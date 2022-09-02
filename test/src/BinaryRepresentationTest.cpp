#include "BinaryRepresentation.h"

#include <gtest/gtest.h>

namespace test {

#define EXPECT_REPRESENTATION(type, value, representation)                          \
    {                                                                               \
        std::ostringstream oss;                                                     \
        solution::print_binary_representation<type>(value, oss);                    \
        EXPECT_EQ(oss.str(), #representation)                                       \
                << "Invalid representation for " << #value << " (" << #type << ")"; \
    }

TEST(BinaryRepresentationTest, uint8_t)
{
    EXPECT_REPRESENTATION(uint8_t, 0, 00000000);
    EXPECT_REPRESENTATION(uint8_t, 1, 00000001);
    EXPECT_REPRESENTATION(uint8_t, 10, 00001010);
    EXPECT_REPRESENTATION(uint8_t, 255, 11111111);
}

#ifdef SIGNED_IN_TWO_S_COMPLEMENT

TEST(BinaryRepresentationTest, int8_t)
{
    EXPECT_REPRESENTATION(int8_t, 0, 00000000);
    EXPECT_REPRESENTATION(int8_t, 1, 00000001);
    EXPECT_REPRESENTATION(int8_t, 10, 00001010);
    EXPECT_REPRESENTATION(int8_t, 127, 01111111);
    EXPECT_REPRESENTATION(int8_t, -0, 00000000);
    EXPECT_REPRESENTATION(int8_t, -1, 11111111);
    EXPECT_REPRESENTATION(int8_t, -10, 11110110);
    EXPECT_REPRESENTATION(int8_t, -128, 10000000);
}

TEST(BinaryRepresentationTest, int16_t)
{
    EXPECT_REPRESENTATION(int16_t, -128, 1111111110000000);
}

#endif

#undef EXPECT_REPRESENTATION

} // namespace test
