#include <constexpr_hash_map/constexpr_hash_map.hpp>

using burda::ct::hash_map;

static constexpr hash_map<1, const char*, int> map{
    std::make_pair("a", 1),
};

// Missing key → compile-time error
static_assert(map["missing"] == 0);

int main() {}
