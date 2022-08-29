![Version](https://img.shields.io/badge/version-1.1.1-blue.svg)
![Language](https://img.shields.io/badge/C++17-blue.svg)
![License](https://img.shields.io/badge/license-MIT_License-blue.svg?style=flat)
[![CI](https://github.com/karel-burda/constexpr-hash-map/actions/workflows/main.yml/badge.svg)](https://github.com/karel-burda/constexpr-hash-map/actions/workflows/main.yml)

# Introduction
Simple single-header compile-time hash-map written in C++ 17.

Hash-map operates only in compile time and represents associative key-value container that contains only unique keys.

It works with custom data-types, if these provide `constexpr` and `noexcept` construction and `operator=`.

Container supports:
* construction in one command
* look-up
* value retrieval
* supports iterators (`cend()`, `std::size()`, ...)
* algorithms (for-each, ...)

Implemented and documented in the [constexpr_hash_map.hpp](include/constexpr_hash_map/constexpr_hash_map.hpp).

Behaviour is undefined, if there are multiple same keys.

Compatible and tested on:
* x86-64 g++ 8.1 and higher
* x86-64 clang++ 6.0 and higher
* x64 MSVC v19.14 and higher

In case of bigger number of elements, compiler's settings regarding constexpr (such as `-fconstexpr-depth` on the GNU) might needed be tuned-up, as container uses compile-time recursion.

# Example
```cpp
#include <constexpr_hash_map/constexpr_hash_map.hpp>

// use arbitrary constexpr constructible types like std::string_view, etc. 
static constexpr burda::ct::hash_map<2, const char *, int> map
{
    std::make_pair("key1", 1),
    std::make_pair("key2", 2)
};

// capacity and iterators
static_assert(map.size() == 2);
static_assert(std::size(map) == 2);
static constexpr auto it = map.find("key1");
static_assert(it != std::cend(map));
static_assert(it->second == 1); // it->second holds the value
static_assert(!std::empty(map));

// accessors
static_assert(map.contains("key1"));
static_assert(!map.contains("key99"));
static_assert(map.at("key1").first); // first denotes, if element was found
static_assert(map.at("key1").second == 1); // second is the actual value
// operator[] doesn't check whether key exists
static_assert(map["key2"] == 2);
// this wouldn't compile -- the key doesn't exist and therefore cannot be evaluated to a constant
// expression, but outside static assertion it would compile and resulted in an undefined behaviour
//static_assert(map["key3"] == 3);

// algorithm support
for (const auto& [key, value] : map)
{
    // do something...
}
```

See also [main.cpp](main.cpp).

Example might compiled (with no additional flags), for example, by this minimal command:
```bash
g++ main.cpp -I include -std=c++17
```

# Live Demo
* ```x86-64 g++ 12.1```: **https://godbolt.org/z/1Ms3s5Wdh**
