#include <string_view>

#include <constexpr_hash_map/constexpr_hash_map.hpp>

int example_simple() noexcept
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

    // just as an example to observe generated assembly
    return map.at("key2").second;
}

int example_advanced() noexcept
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

    // dummy example to observe constexpr nature of the generated assembly
    return it->second.size();
}

int main(const int, const char**)
{
    return example_simple() + example_advanced();
}
