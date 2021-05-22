#pragma once

#include <array>
#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <utility>

namespace burda::ct
{
template <std::size_t N, typename K, typename V>
class hash_map
{
public:
    template<typename... E>
    explicit constexpr hash_map(E&&... elements)
    : data{std::forward<E>(elements)...}
    {
        static_assert(N > 0, "N should be positive");
        static_assert(N == sizeof...(elements), "Elements size don't match expected size of hash-map");
    }

    [[nodiscard]] constexpr const V& at(const K& key) const
    {
        const auto it = search<0, N>(key);

        if (it != std::cend(data))
            return it->second;

        throw std::out_of_range{"Element not found"};
    }

    [[nodiscard]] constexpr const V& operator[](const K& key) const
    {
        return at(key);
    }

    [[nodiscard]] constexpr bool contains(const K& key) const noexcept
    {
        return search<0, N>(key) != std::cend(data);
    }

    [[nodiscard]] constexpr std::size_t size() const noexcept
    {
        return data.size();
    }

private:
    using index_t = std::size_t;

    template <index_t L, index_t R>
    [[nodiscard]] constexpr auto search(const K& key) const
    {
        if constexpr (L < R)
        {
            if (data[L].first == key)
            {
                return std::next(std::cbegin(data), L);
            }

            return search<L+1, R>(key);
        }

        return std::cend(data);
    }

    std::array<std::pair<K, V>, N> data;
};
}
