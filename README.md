![Version](https://img.shields.io/badge/C++17-blue.svg)
![Build](https://www.travis-ci.com/karel-burda/constexpr-hash-map.svg?branch=main)
[![License](https://img.shields.io/badge/license-MIT_License-blue.svg?style=flat)](LICENSE)

# Introduction
Simple single-header compile-time hash-map written in C++ 17.

Compatible and tested on:
* x86-64 g++ 10.1 and higher
* x64 MSVC v19.14 and higher

Implented in [constexpr_hash_map.hpp](include/constexpr_hash_map/constexpr_hash_map.hpp).

# Example
```cpp
#include <constexpr_hash_map/constexpr_hash_map.hpp>

static constexpr burda::ct::hash_map<2, const char *, int> map
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
// outside static assertion, it would throw std::out_of_range
```

See also [main.cpp](main.cpp).

# Live Demo
* ```x86-64 g++ 10.1, -std=c++17 -O1 -Wall -Wextra -pedantic```: **https://godbolt.org/z/3f3s6jxhY**
* ```x64 MSVC v19.14, /std:c++17 /O2```: **https://godbolt.org/z/rWYKGW6vc**
