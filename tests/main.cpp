#include <string_view>

#include <constexpr_hash_map/constexpr_hash_map.hpp>

namespace burda::test
{
[[nodiscard]] constexpr int simple() noexcept
{
    static constexpr burda::ct::hash_map<2, const char*, int> map
    {
        std::make_pair("key1", 1),
        std::make_pair("key2", 2)
    };

    static_assert(map.size() == 2);
    static_assert(map.contains("key1"));
    static_assert(map.at("key1").second == 1);
    static_assert(map.contains("key2"));
    static_assert(map["key2"] == 2);
    static_assert(!map.contains("key3"));
    // this would not compile, because because hash map doesn't contain it
    //static_assert(map["key3"] == 3);

    return map.at("key2").second == 2;
}

[[nodiscard]] constexpr int advanced() noexcept
{
    static constexpr burda::ct::hash_map<3, std::string_view, std::string_view> map
    {
        std::make_pair("key1", "value1"),
        std::make_pair("key2", "value2"),
        std::make_pair("key3", "value3")
    };

    // container supports iterators in a basic way
    static constexpr auto it = map.find("key2");
    static_assert(it != std::cend(map));
    static_assert(it->second == "value2");
    // another example calls
    static_assert(!std::empty(map));
    static_assert(std::size(map) == 3);

    for ([[maybe_unused]] const auto& [key, value] : map)
    {
        // do something with the elements
    }

    return it->second.size() == 6;
}

[[nodiscard]] constexpr int run()
{
  return simple() + advanced();
}
} // namespace burda::test

int main([[maybe_unused]] const int argc, [[maybe_unused]] const char** argv)
{
    return burda::test::run();
}
