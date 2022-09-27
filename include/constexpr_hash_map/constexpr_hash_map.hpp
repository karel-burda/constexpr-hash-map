#ifndef BURDA_CONSTEXPR_HASH_MAP_HPP
#define BURDA_CONSTEXPR_HASH_MAP_HPP

#include <array>
#include <cstddef>
#include <iterator>
#include <utility>

namespace burda::ct
{
/// @brief Compile-time hash-map (associative key-value container) that performs all operations in constexpr context.
///        This means that keys and values have to be constexpr and noexcept constructible and provide constexpr noexcept operator=.
/// @brief Behaviour is undefined, if there are multiple same keys.
/// @brief There's actually no hash function needed, see details section.
/// @details Implemented as an std::array containing pairs, so no hashing is involved.
/// @tparam N total number of elements
/// @tparam K data type for keys
/// @tparam V data type for values
template <std::size_t N, typename K, typename V>
class hash_map
{
public:
    /// @see std::unordered_map<...>::key_type
    using key_type = K;
    /// @see std::unordered_map<...>::value_type
    using value_type = V;
    /// @see std::unordered_map<...>::size_type
    using size_type = decltype(N);
    /// @brief underlying structure used for the actual implementation
    using data_type = std::array<std::pair<K, V>, N>;
    /// @see std::array<...>::const_iterator
    using const_iterator = typename data_type::const_iterator;

    /// @brief The only construction that might be used, all keys and values must be provided in the constructor
    /// @tparam R Variadic arguments automatically deduced by the compiler
    /// @param elements series of std::pair<K, V>, cannot be empty
    template<typename... E>
    explicit constexpr hash_map(E&&... elements) noexcept
    : data{std::forward<E>(elements)...}
    {
        static_assert(N > 0, "N should be positive");
        static_assert(N == sizeof...(elements), "Elements size doesn't match expected size of a hash-map");
    }

    /// @brief Searches map for a given key and returns iterator
    /// @param key key to be searched for
    /// @return constant iterator to an element (cend, if not found)
    [[nodiscard]] constexpr const_iterator find(const K& key) const noexcept
    {
        return search<0, N>(key);
    }

    /// @brief Searches for a given key, aimed to return associated value with it
    /// @param key key to be searched for
    /// @return pair, where first denotes whether element was found, second given value
    /// @details Deliberately not throwing an exception, and returning pair instead,
    ///          as this generates much shorter assembly on clang and msvc
    [[nodiscard]] constexpr std::pair<bool, const V&> at(const K& key) const noexcept
    {
        const auto it = find(key);

        if (it != cend())
        {
            return {true, it->second};
        }

        return {false, {}};
    }

    /// @brief Retrieves reference to constant to a value.
    ///        Doesn't perform any bounds checking, behaviour is undefined if the key doesn't exist
    /// @param key key to be searched for
    /// @return reference to constant to a value associated with the key
    // NOLINTNEXTLINE(fuchsia-overloaded-operator)
    [[nodiscard]] constexpr const V& operator[](const K& key) const noexcept
    {
        return find(key)->second;
    }

    /// @brief Checks if element with given key exists
    /// @param key key to be searched for
    /// @return boolean that denotes key's existence
    [[nodiscard]] constexpr bool contains(const K& key) const noexcept
    {
        return search<0, N>(key) != cend();
    }

    /// @brief Retrieves size of a hash-map, might also be called indirectly using the std::size(...)
    /// @return total size of a container
    [[nodiscard]] constexpr size_type size() const noexcept
    {
        return data.size();
    }

    /// @brief Gives constant iterator to a beginning, needed for the C++11 for-each cycle or the std::for_each
    /// @return constant iterator to a beginning
    /// @see cbegin()
    [[nodiscard]] constexpr const_iterator begin() const noexcept
    {
        return cbegin();
    }

    /// @brief Gives constant iterator to a beginning, might be also called using std::cbegin(...)
    /// @return constant iterator to a beginning
    /// @see std::unordered_map<...>::cbegin()
    [[nodiscard]] constexpr const_iterator cbegin() const noexcept
    {
        return std::cbegin(data);
    }

    /// @brief Gives constant iterator to an end, needed for the C++11 for-each cycle or the std::for_each
    /// @return constant iterator to an end (past the last element)
    /// @see cend()
    [[nodiscard]] constexpr const_iterator end() const noexcept
    {
        return cend();
    }

    /// @brief Gives constant iterator to a beginning, might be also called using std::cend(...)
    /// @return constant iterator to an end (past the last element)
    /// @see std::unordered_map<...>::cend()
    [[nodiscard]] constexpr const_iterator cend() const noexcept
    {
        return std::cend(data);
    }

    /// @brief Hash-map cannot be empty; this might also be called using std::empty(...)
    /// @return false -- cannot be empty
    /// @see std::unordered_map<...>::empty()
    [[nodiscard]] constexpr bool empty() const noexcept
    {
        return false;
    }

protected:
    /// @private type used for indexing elements
    using index_type = size_type;

    /// @private function that does a constexpr recursive searching of the array from left to right
    template <index_type L, index_type R>
    [[nodiscard]] constexpr const_iterator search(const K& key) const noexcept
    {
        if constexpr (L < R)
        {
            if (equal(data[L].first, key))
            {
                return std::next(cbegin(), L);
            }

            return search<L+1, R>(key);
        }

        return cend();
    }

    /// @private generic implementation that compares keys
    template <typename T = K>
    [[nodiscard]] constexpr bool equal(const T& lhs, const T& rhs) const noexcept
    {
        return lhs == rhs;
    }

    /// @private specific implementation for "const char*" equality is needed; uses recursion
    [[nodiscard]] constexpr bool equal(const char* lhs, const char* rhs) const noexcept
    {
        return *lhs == *rhs && (*lhs == '\0' || equal(lhs + 1, rhs + 1));
    }

private:
    data_type data;
};
}  // namespace burda::ct

#endif // BURDA_CONSTEXPR_HASH_MAP_HPP
