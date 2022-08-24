#ifndef BURDA_CONSTEXPR_HASH_MAP_HPP
#define BURDA_CONSTEXPR_HASH_MAP_HPP

#include <array>
#include <cstddef>
#include <iterator>
#include <utility>

namespace burda::ct
{
///
/// @brief Compile-time hash-map (associative key-value container) that performs all operations in constexpr context.
///        This means that keys and values has to be constexpr constructible and provide constexpr equals operator.
///       Behaviour is undefined, if there are multiple keys.
///       There's actually no hash function needed, see details section.
/// @details Implemented as an std::array containng pairs, so no hashing involved.
/// @tparam N total number of elements
/// @tparam K data type for keys
/// @tparam V data type for values
///
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

    ///
    /// @brief The only construction that might be used, all keys and values must be provided in the constructor
    /// @tparam R Variadic arguments automatically deduced by the compiler
    /// @param elements series of std::pair<K, V>, cannot be empty
    ///
    template<typename... E>
    explicit constexpr hash_map(E&&... elements) noexcept
    : data{std::forward<E>(elements)...}
    {
        static_assert(N > 0, "N should be positive");
        static_assert(N == sizeof...(elements), "Elements size doesn't match expected size of a hash-map");
    }

    ///
    /// @brief Searches map for a given key and returns iterator
    /// @param key key to be searched for
    /// @return constant iterator to an element (cend, if not found)
    ///
    [[nodiscard]] constexpr const_iterator find(const K& key) const noexcept
    {
        return search<0, N>(key);
    }

    ///
    /// @brief Searches for a given key, aimed to return associated value with it
    /// @param key key to be searched for
    /// @return pair, where first denotes whether element was found, second given value
    /// @details Deliberately not throwing an exception, and returning pair instead,
    ///          as this generates much shorter assembly on clang and msvc
    ///
    [[nodiscard]] constexpr std::pair<bool, const V&> at(const K& key) const noexcept
    {
        const auto it = find(key);

        if (it != cend())
        {
            return {true, it->second};
        }

        return {false, {}};
    }

    ///
    /// @brief Retrieves reference to constant to a value.
    ///        Doesn't perform any bounds checking, behaviour is undefined if the key doesn't exist
    /// @param key key to be searched for
    /// @return reference to constant to a value
    ///
    [[nodiscard]] constexpr const V& operator[](const K& key) const noexcept
    {
        return find(key)->second;
    }

    [[nodiscard]] constexpr bool contains(const K& key) const noexcept
    {
        return search<0, N>(key) != std::cend(data);
    }

    [[nodiscard]] constexpr size_type size() const noexcept
    {
        return data.size();
    }

    // need for for-each cycle
    [[nodiscard]] constexpr const_iterator begin() const noexcept
    {
        return cbegin();
    }

    [[nodiscard]] constexpr const_iterator cbegin() const noexcept
    {
        return std::cbegin(data);
    }

    // need for for-each cycle
    [[nodiscard]] constexpr const_iterator end() const noexcept
    {
        return cend();
    }

    [[nodiscard]] constexpr const_iterator cend() const noexcept
    {
        return std::cend(data);
    }

    [[nodiscard]] constexpr bool empty() const noexcept
    {
        return false;
    }

protected:
    using index_type = size_type;

    template <index_type L, index_type R>
    [[nodiscard]] constexpr const_iterator search(const K& key) const noexcept
    {
        if constexpr (L < R)
        {
            if (equal(data[L].first, key))
            {
                return std::next(std::cbegin(data), L);
            }

            return search<L+1, R>(key);
        }

        return std::cend(data);
    }

    template <typename T = K>
    [[nodiscard]] constexpr bool equal(const T& a, const T& b) const noexcept
    {
        return a == b;
    }

    // specific implementation for const char* is needed
    [[nodiscard]] constexpr bool equal(char const * a, char const * b) const noexcept
    {
        return *a == *b && (*a == '\0' || equal(a + 1, b + 1));
    }

private:
    data_type data;
};
}

#endif // BURDA_CONSTEXPR_HASH_MAP_HPP
