#include "Serialization.h"

#include <optional>
#include <unordered_map>
#include <vector>

namespace solution {

List::List()
    : m_head(&m_tail)
    , m_size(0)
{
}

List::Iterator List::insert(ConstIterator position, const std::string_view data)
{
    auto * node = position.m_pointer;
    auto * inserted = new ListNode();
    inserted->m_data = data;

    inserted->m_next = node;
    inserted->m_previous = node->m_previous;

    if (node->m_previous) {
        node->m_previous->m_next = inserted;
    }
    else {
        m_head = inserted;
    }
    node->m_previous = inserted;

    ++m_size;

    return Iterator{&m_tail, inserted};
}

List::Iterator List::push_front(std::string_view data)
{
    return insert(begin(), data);
}

List::Iterator List::push_back(std::string_view data)
{
    return insert(end(), data);
}

List::Iterator List::erase(ConstIterator position)
{
    auto * node = position.m_pointer;
    auto * next = node->m_next;
    auto * previous = node->m_previous;
    if (next) {
        next->m_previous = previous;
    }
    if (previous) {
        previous->m_next = next;
    }
    else {
        m_head = next;
    }
    delete node;
    --m_size;
    return Iterator{&m_tail, next};
}

List::Iterator List::pop_front()
{
    return erase(begin());
}

List::Iterator List::pop_back()
{
    return erase(std::prev(end()));
}

List::Iterator List::begin() noexcept
{
    return Iterator{&m_tail, m_head};
}

List::ConstIterator List::begin() const noexcept
{
    return ConstIterator{&m_tail, m_head};
}

List::Iterator List::end() noexcept
{
    return Iterator{&m_tail, &m_tail};
}

List::ConstIterator List::end() const noexcept
{
    return ConstIterator{&m_tail, &m_tail};
}

void List::clear()
{
    while (!empty()) {
        pop_front();
    }
}

namespace serializers {

template <typename T>
void serialize(const T & value, file_ptr_t file)
{
    std::fwrite(&value, sizeof(T), 1, file);
}

template <>
void serialize<std::string>(const std::string & str, file_ptr_t file)
{
    std::fwrite(str.c_str(), sizeof(char), str.length() + 1, file);
}

} // namespace serializers

int List::serialize(file_ptr_t file)
{
    if (!file) {
        return -1;
    }
    std::vector<const ListNode *> nodes;
    std::unordered_map<const ListNode *, std::size_t> indexes;
    std::size_t index = 0;
    for (auto it = begin(); it != end(); ++index, ++it) {
        const auto * node = it.m_pointer;
        nodes.push_back(node);        // Amortized O(1)
        indexes.emplace(node, index); // Amortized O(1)
    }                                 // O(n)

    for (const auto * node : nodes) {
        serializers::serialize(node->m_data, file);
        auto random = NO_RANDOM;
        if (node->m_random) {
            random = indexes[node->m_random];
        }
        serializers::serialize(random, file);
    } // O(n)

    return 0;
}

namespace deserializers {

template <typename T>
std::optional<T> deserialize(file_ptr_t file)
{
    T value;
    if (std::fread(&value, sizeof(T), 1, file) != 1) {
        return std::nullopt;
    }
    return value;
}

template <>
std::optional<std::string> deserialize<std::string>(file_ptr_t file)
{
    std::vector<char> buffer;
    char c;
    while ((c = std::getc(file)) != '\0') {
        if (c == EOF) {
            return std::nullopt;
        }
        buffer.push_back(c);
    }
    return std::string{buffer.data(), buffer.size()};
}

} // namespace deserializers

int List::deserialize(file_ptr_t file)
{
    if (!file) {
        return -1;
    }
    struct NodeRecord
    {
        NodeRecord(const std::string & data, std::size_t random)
            : m_data(data)
            , m_random(random)
        {
        }
        std::string m_data;
        std::size_t m_random;
    };
    std::vector<NodeRecord> records;
    while (const auto data = deserializers::deserialize<std::string>(file)) {
        const auto random = deserializers::deserialize<std::size_t>(file);
        if (!random) {
            return 1;
        }
        records.emplace_back(data.value(), random.value()); // Amortized O(1)
    }                                                       // O(n)
    clear();                                                // O(n)
    std::vector<ListNode *> nodes;
    for (const auto & record : records) {
        const auto iterator = push_back(record.m_data); // O(1)
        nodes.push_back(iterator.m_pointer);            // Amortized O(1)
    }                                                   // O(n)
    for (std::size_t i = 0; i < nodes.size(); ++i) {
        const auto random = records[i].m_random;
        if (random == NO_RANDOM) {
            continue;
        }
        nodes[i]->m_random = nodes[random];
    } // O(n)
    return 0;
}

} // namespace solution
