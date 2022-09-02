#include "BinaryRepresentation.h"
#include "RemovingDuplicates.h"
#include "Serialization.h"

int main()
{
    std::cout << "1. Binary representation of 10 (uint8_t) is ";
    solution::print_binary_representation<uint8_t>(10);
    std::cout << "\n" << std::endl;

    char data[] = "AAA BBB AAA";
    solution::remove_duplicates(data);
    printf("2. Remove duplicates from 'AAA BBB AAA': %s\n\n", data);

    solution::List list;
    for (const std::string_view value : {"1", "2", "3", "4"}) {
        list.push_back(value);
    }
    auto first = list.begin();
    auto second = std::next(first);
    second.link(first);

    auto * file = std::fopen("buffer", "wb");
    list.serialize(file);
    std::fclose(file);

    list.clear();

    file = std::fopen("buffer", "rb");
    list.deserialize(file);
    std::fclose(file);

    std::cout << "3. List {'1', '2', '3', '4'} ('2' linked with '1') after deserialization: ["
              << list.size() << "] {" << std::endl;
    for (auto it = list.begin(); it != list.end(); ++it) {
        std::cout << "    '" << *it << "' -> "
                  << (it.next() == list.end() ? "nullptr" : "'" + *it.next() + "'") << std::endl;
    }
    std::cout << "}" << std::endl;

    return 0;
}
