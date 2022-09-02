#include "RemovingDuplicates.h"

#include <cstring>
#include <gtest/gtest.h>

namespace test {

TEST(RemovingDuplicatesTest, empty)
{
    {
        char data[] = "";
        solution::remove_duplicates(data);
        EXPECT_EQ(std::strcmp(data, ""), 0);
    }
    {
        char data[] = "    ";
        solution::remove_duplicates(data);
        EXPECT_EQ(std::strcmp(data, " "), 0);
    }
}

TEST(RemovingDuplicatesTest, one_letter)
{
    {
        char data[] = "AAAAAAAAAAAAA";
        solution::remove_duplicates(data);
        EXPECT_EQ(std::strcmp(data, "A"), 0);
    }
}

TEST(RemovingDuplicatesTest, sample)
{
    {
        char data[] = "AAA BBB AAA";
        solution::remove_duplicates(data);
        EXPECT_EQ(std::strcmp(data, "A B A"), 0);
    }
}

} // namespace test
