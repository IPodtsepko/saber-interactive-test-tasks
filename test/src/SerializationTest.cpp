#include "Serialization.h"

#include <gtest/gtest.h>

namespace test {

TEST(SerializationTest, insert)
{
    solution::List list;

    ASSERT_EQ(*list.push_back("2"), "2");
    ASSERT_EQ(*list.push_front("1"), "1");

    ASSERT_EQ(list.size(), 2);

    auto it = list.begin();
    ASSERT_EQ(*(it++), "1");
    ASSERT_EQ(*(it++), "2");
    ASSERT_EQ(it, list.end());

    ASSERT_EQ(*(--it), "2");
}

TEST(SerializationTest, remove)
{
    solution::List list;

    list.push_back("1");
    ASSERT_EQ(list.pop_front(), list.end());
    ASSERT_TRUE(list.empty());

    list.push_back("1");
    list.push_back("2");
    list.push_back("3");
    ASSERT_EQ(list.size(), 3);

    ASSERT_EQ(*list.pop_front(), "2");
    ASSERT_EQ(list.size(), 2);

    auto it = list.begin();
    ASSERT_EQ(*(it++), "2");
    ASSERT_EQ(*(it++), "3");
    ASSERT_EQ(it, list.end());

    ASSERT_EQ(list.pop_back(), list.end());
    ASSERT_EQ(list.size(), 1);

    it = list.begin();
    ASSERT_EQ(*(it++), "2");
    ASSERT_EQ(it, list.end());

    list.push_back("1");
    list.push_front("3");

    it = list.begin();
    ASSERT_EQ(*(it++), "3");
    ASSERT_EQ(*(it++), "2");
    ASSERT_EQ(*(it++), "1");
    ASSERT_EQ(it, list.end());

    list.clear();
    ASSERT_EQ(list.size(), 0);
}

TEST(SerializationTest, end)
{
    solution::List list;

    list.push_front("1");
    list.push_back("2");

    const auto end = list.end();
    ASSERT_EQ(*std::prev(end), "2");

    list.push_back("3");
    ASSERT_EQ(*std::prev(end), "3");
}

TEST(SerializationTest, random_order)
{
    solution::List list;

    for (const std::string_view value : {"1", "2", "3", "4"}) {
        list.push_back(value);
    }

    auto first = list.begin();
    auto second = std::next(first);
    auto third = std::next(second);
    auto fourth = std::next(third);

    first.link(fourth);
    second.link(third);
    fourth.link(second);

    auto it = first;
    ASSERT_EQ(*it, "1");
    ASSERT_EQ(*(it.move()), "4");
    ASSERT_EQ(*(it.move()), "2");
    ASSERT_EQ(*(it.move()), "3");
    ASSERT_EQ(it.move(), list.end());
    ASSERT_EQ(*std::prev(it), "4");
}

TEST(SerializationTest, serialization)
{
    solution::List list;
    for (const std::string_view value : {
                 "Hello, world!",
                 "This is second element.",
                 "This is first line,\nThis is second line.",
                 "Last value."}) {
        list.push_back(value);
    }
    auto first = list.begin();
    auto second = std::next(first);
    second.link(first);

    auto * file = std::fopen("buffer", "wb");
    ASSERT_EQ(list.serialize(file), 0);
    std::fclose(file);

    file = std::fopen("buffer", "rb");
    solution::List deserialized;
    deserialized.push_back("This should be removed");
    ASSERT_EQ(deserialized.deserialize(file), 0);
    std::fclose(file);

    ASSERT_EQ(list.size(), deserialized.size());
    auto it = deserialized.begin();
    for (auto jt = list.begin(); jt != list.end(); ++it, ++jt) {
        ASSERT_EQ(*it, *jt);
        ASSERT_EQ(*it.next(), *jt.next());
    }
    ASSERT_EQ(it, deserialized.end());

    first = deserialized.begin();
    second = std::next(first);

    ASSERT_EQ(second.next(), first);
}

TEST(SerializationTest, strict_guarantees)
{
    solution::List list;

    for (const std::string_view value : {"1", "2", "3"}) {
        list.push_back(value);
    }

    ASSERT_NE(list.deserialize(nullptr), 0);
    ASSERT_EQ(list.size(), 3);

    auto it = list.begin();
    ASSERT_EQ(*(it++), "1");
    ASSERT_EQ(*(it++), "2");
    ASSERT_EQ(*(it++), "3");
    ASSERT_EQ(it, list.end());

    const char invalid_serialization[] = "Just string (without index)";
    auto * file = std::fopen("buffer", "wb");
    std::fwrite(invalid_serialization, sizeof(char), 28, file);
    std::fclose(file);

    file = std::fopen("buffer", "rb");
    ASSERT_NE(list.deserialize(file), 0);
    std::fclose(file);

    ASSERT_EQ(list.size(), 3);

    it = list.begin();
    ASSERT_EQ(*(it++), "1");
    ASSERT_EQ(*(it++), "2");
    ASSERT_EQ(*(it++), "3");
    ASSERT_EQ(it, list.end());
}

} // namespace test
