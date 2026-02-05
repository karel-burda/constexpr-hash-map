#include <constexpr_hash_map/constexpr_hash_map.hpp>

using burda::ct::hash_map;

/*
 * Basic construction and lookup
 */
static constexpr hash_map<2, const char*, int> basic_map{
    std::make_pair("a", 1),
    std::make_pair("b", 2),
};

static_assert(basic_map.size() == 2, "size() failed");
static_assert(basic_map.contains("a"), "contains() failed");
static_assert(basic_map.contains("b"), "contains() failed");
static_assert(!basic_map.contains("c"), "contains() false positive");

/*
 * Safe lookup: get()
 */
static_assert(basic_map.get("a") != nullptr, "get() returned nullptr");
static_assert(*basic_map.get("a") == 1, "get() returned wrong value");
static_assert(basic_map.get("c") == nullptr, "get() should return nullptr");

/*
 * Safe lookup: value_or()
 */
static_assert(basic_map.value_or("a", -1) == 1, "value_or() failed");
static_assert(basic_map.value_or("c", -1) == -1, "value_or() fallback failed");

/*
 * operator[] with existing keys
 */
static_assert(basic_map["a"] == 1, "operator[] failed");
static_assert(basic_map["b"] == 2, "operator[] failed");

/*
 * Iteration sanity check (order-independent)
 */
constexpr bool iteration_test() {
    bool seen_a = false;
    bool seen_b = false;

    for (const auto& kv : basic_map) {
        if (kv.first == std::string_view("a") && kv.second == 1)
            seen_a = true;
        if (kv.first == std::string_view("b") && kv.second == 2)
            seen_b = true;
    }
    return seen_a && seen_b;
}

static_assert(iteration_test(), "iteration failed");

/*
 * at() API (if present)
 */
constexpr bool at_test() {
    const auto r1 = basic_map.at("a");
    const auto r2 = basic_map.at("c");

    return r1.first && r1.second == 1
        && !r2.first;
}

static_assert(at_test(), "at() failed");

int main() {}
