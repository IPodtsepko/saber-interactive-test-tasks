#include <limits>
#include <string>

namespace solution {

using file_ptr_t = FILE *;

/**
 * Serializable Doubly Linked List.
 *
 * Explanation for implementation.
 *  - I support the fake node pointed to by tail. This structure
 *    allows you to simplify the implementation of some methods due
 *    to the removal of common code. For example, Insertion at the
 *    end and at the beginning in this case is generalized into an
 *    insertion operation before some node (before the fake or
 *    before the head). In addition, the implementation of the property
 *    is simplified, in which the iterator to the end of the list is
 *    easy to decrement and always get the last element.
 *
 * @author Igor Podtsepko (i.podtsepko2002@gmail.com)
 */
class List
{
public:
    List();

private:
    struct ListNode
    {
        ListNode * m_previous = nullptr;
        ListNode * m_next = nullptr;
        ListNode * m_random = nullptr;
        std::string m_data;
    };

public:
    /**
     * Iterator generalization (abstraction that does not take into account constness).
     */
    template <typename T>
    struct IteratorT
    {
        friend class List;

        using difference_type = std::ptrdiff_t;
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using reference = T &;
        using pointer = T *;

        /**
         * Constructor that allows you to cast a non-const iterator to a const iterator.
         */
        template <typename U, typename = std::enable_if_t<std::is_const_v<T> && !std::is_const_v<U>>>
        IteratorT(const IteratorT<U> & iterator)
            : m_pointer(iterator.m_pointer)
        {
        }

        pointer operator->() const
        {
            return &m_pointer->m_data;
        }

        reference operator*() const
        {
            return m_pointer->m_data;
        }

    private:
        /**
         * A helper function that makes it easier to determine the next node in a "random" order.
         *
         * @return pointer to the next node in "random" order.
         */
        ListNode * next(ListNode * current) noexcept
        {
            auto * next = current->m_random;
            if (!next) {
                return m_tail;
            }
            return next;
        }

    public:
        /**
         * Moves to next in "random" order.
         *
         * @return iterator to the next in "random" order.
         */
        IteratorT & move()
        {
            m_pointer = next(m_pointer);
            return *this;
        }

        /**
         * @return iterator to the next in "random" order.
         */
        IteratorT next()
        {
            return IteratorT{m_tail, next(m_pointer)};
        }

        /**
         * Updates "random" order.
         */
        void link(const IteratorT & random)
        {
            m_pointer->m_random = random.m_pointer;
        }

        IteratorT & operator++()
        {
            m_pointer = m_pointer->m_next;
            return *this;
        }

        IteratorT operator++(int)
        {
            auto tmp = *this;
            ++*this;
            return tmp;
        }

        IteratorT & operator--()
        {
            m_pointer = m_pointer->m_previous;
            return *this;
        }

        IteratorT operator--(int)
        {
            auto tmp = *this;
            --*this;
            return tmp;
        }

        friend bool operator==(const IteratorT & lhs, const IteratorT & rhs)
        {
            return lhs.m_pointer == rhs.m_pointer;
        }

        friend bool operator!=(const IteratorT & lhs, const IteratorT & rhs)
        {
            return !(lhs == rhs);
        }

    private:
        ListNode * m_tail;
        ListNode * m_pointer;

        explicit IteratorT(ListNode * tail, ListNode * pointer)
            : m_tail(tail)
            , m_pointer(pointer)
        {
        }
    };

public:
    using Iterator = IteratorT<std::string>;
    using ConstIterator = IteratorT<const std::string>;

    /**
     * Complexity: O(1)
     * @return true is the list has no elements.
     */
    bool empty() const noexcept { return size() == 0; }
    /**
     * Complexity: O(1)
     * @return the number of elements in the list.
     */
    std::size_t size() const noexcept { return m_size; }

    /**
     * Inserts data before position.
     * Complexity: O(1)
     *
     * @param position iterator before which the data will be inserted.
     * @param data data to insert.
     * @return iterator to the inserted element.
     */
    Iterator insert(ConstIterator position, std::string_view data);

    /**
     * Prepends the given data to the beginning of the list.
     *
     * @param data data to insert.
     * @return iterator to the inserted element.
     */
    Iterator push_front(std::string_view data);
    /**
     * Complexity: O(1)
     * Appends the given data to the ending of the list.
     *
     * @param data data to insert.
     * @return iterator to the inserted element.
     */
    Iterator push_back(std::string_view data);

    /**
     * Complexity: O(1)
     * Removes the data at position.
     *
     * @param position iterator to the element to remove.
     * @return iterator following the removed element.
     */
    Iterator erase(ConstIterator position);

    /**
     * Removes the data at the beginning of the list.
     * Complexity: O(1)
     *
     * @return iterator following the removed element.
     */
    Iterator pop_front();
    /**
     * Removes the data at the ending of the list.
     * Complexity: O(1)
     *
     * @return iterator following the removed element.
     */
    Iterator pop_back();

    /**
     * Erases all elements from the list.
     * Complexity: O(n)
     */
    void clear();

    /**
     * Complexity: O(1)
     * @return an iterator to the beginning.
     */
    Iterator begin() noexcept;
    /**
     * Complexity: O(1)
     * @return an iterator to the beginning.
     */
    ConstIterator begin() const noexcept;

    /**
     * Complexity: O(1)
     * @return an iterator to the end.
     */
    Iterator end() noexcept;
    /**
     * Complexity: O(1)
     * @return an iterator to the end.
     */
    ConstIterator end() const noexcept;

public:
    /**
     * Serializes a list and writes the result to a binary file.
     * Complexity: O(n)
     *
     * @param file the file to which the list should be serialized.
     * @return 0 if list serialized successfully, else - non zero error code.
     */
    int serialize(file_ptr_t file);
    /**
     * Deserializes the list according to the data from the binary file.
     * Complexity: O(n)
     *
     * @param file the file from which the list should be deserialized.
     * @return 0 if list deserialized successfully, else - non zero error code.
     */
    int deserialize(file_ptr_t file);

private:
    mutable ListNode m_tail;
    ListNode * m_head;
    std::size_t m_size;

    /**
     * Special value for encoding null pointer in serialization.
     */
    static constexpr auto NO_RANDOM = std::numeric_limits<std::size_t>::max();
};

} // namespace solution
