#include <constexpr_hash_map/constexpr_hash_map.hpp>

int main(int, const char**)
{
    constexpr const burda::ct::hash_map<2, const char*, int> map
    {
        std::make_pair("key1", 1),
        std::make_pair("key2", 2)
    };

    static_assert(map.size() == 2);
    static_assert(map.contains("key1"));
    static_assert(map.at("key1") == 1);
    static_assert(map.contains("key2"));
    static_assert(map["key2"] == 2);
    static_assert(!map.contains("key3"));
    // this would not compile, because because hash map doesn't contain it
    //static_assert(map["key3"] == 3);

    // just as an example to observe generated assembly
    return map.at("key2");
}
