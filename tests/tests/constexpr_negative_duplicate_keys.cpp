#include <constexpr_hash_map/constexpr_hash_map.hpp>

using burda::ct::hash_map;

static constexpr hash_map<2, const char*, int> duplicate_keys{
    std::make_pair("x", 1),
    std::make_pair("x", 2), // duplicate key → compile-time error
};

int main() {}
