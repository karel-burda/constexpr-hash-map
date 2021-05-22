![Version](https://img.shields.io/badge/C++17-blue.svg)
[![License](https://img.shields.io/badge/license-MIT_License-blue.svg?style=flat)](LICENSE)

# Introduction
Simple single-header compile-time hash-map written in C++ 17.

Compatible with g++ 10.1 and higher.

Implented in [constexpr_hash_map.hpp](include/constexpr_hash_map/constexpr_hash_map.hpp).

# Example
```cpp
constexpr const burda::ct::hash_map<2, const char *, int> map
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
// in runtime, it throws `std::out_of_range`
```

See also [main.cpp](main.hpp).

# Live Demo
* ```g++ 10.1, -std=c++17 -O1 -Wall -Wextra -pedantic```: **https://godbolt.org/z/vc4oshr1j**
