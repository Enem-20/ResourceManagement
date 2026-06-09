/**
 * @file   Resourceable.hpp
 * @brief  Header traits, hashing and resourceable requirements generator(GENERATE_RESOURCEABLE macro) for ResourceManager library.
 *
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Sergey Zamaro
 */

#ifndef C_RESOURCEABLE_HPP
#define C_RESOURCEABLE_HPP

#include <type_traits>

#include <string>
#include <cstdint>
#include <vector>


#if __cpp_concepts >= 201907L
#include <concepts>

template<class T>
concept is_resourceable = requires(T obj) {
	{T::typeHash} -> std::convertible_to<uint64_t>;
	{T::type} -> std::convertible_to<std::string_view>;
    {obj.getNameHash()} -> std::convertible_to<uint64_t>;
    {obj.getName()} -> std::convertible_to<std::string_view>;
    {obj.setName(std::string{})} -> std::convertible_to<void>;
    {obj.getPath()} -> std::convertible_to<std::string>;
    {obj.setPath(std::string{})} -> std::convertible_to<void>;
	{T::deserialize(std::vector<std::byte>{})} -> std::convertible_to<T*>;
    {obj.serialize()} -> std::convertible_to<std::vector<std::byte>>;
};

#elif (defined(__cplusplus) && __cplusplus >= 201703L) || (defined(_MSVC_LANG) && _MSVC_LANG >= 201703L)
    #if defined(__has_include)
        #if __has_include(<type_traits>) && __has_include(<utility>)
            #include <type_traits>
            #include <utility>

            #if defined(__cpp_lib_logical_traits) && __cpp_lib_logical_traits >= 201510L
                #include "Resourceable17.hpp"
            #endif
        #endif
    #endif
#endif

constexpr auto fnv1a_32(const char* str, std::size_t len) -> std::uint32_t {
    constexpr uint32_t initialHash = 2166136261U;
    constexpr uint32_t salt = 16777619U;
    std::uint32_t hash = initialHash;
    for (std::size_t i = 0; i < len; ++i) {
        hash ^= static_cast<std::uint32_t>(str[i]);
        hash *= salt;
    }
    return hash;
}

constexpr auto fnv1a_64(const char* str, std::size_t len) noexcept -> std::uint64_t {
    constexpr uint64_t initialHash = 14695981039346656037ULL;
    constexpr uint64_t salt = 1099511628211ULL;
    std::uint64_t hash = initialHash;
    for (std::size_t i = 0; i < len; ++i) {
        hash ^= static_cast<std::uint64_t>(str[i]);
        hash *= salt;
    }
    return hash;
}

constexpr auto hash_string(std::string_view sv) noexcept {
    return fnv1a_64(sv.data(), sv.size());
}

#define GENERATE_RESOURCEABLE(className, dataMembers)                               \
protected:                                                                          \
    struct Data {                                                                   \
        std::string name;                                                           \
        std::string path;                                                           \
        dataMembers                                                                 \
    };                                                                              \
    Data _data;                                                                     \
                                                                                    \
public:                                                                             \
    static constexpr std::string_view type = #className;                            \
    static constexpr uint64_t typeHash = hash_string(#className);                   \
                                                                                    \
    void setName(const std::string& name) { _data.name = name; }                    \
    std::string_view getName() const { return _data.name; }                         \
    uint64_t getNameHash() const { return hash_string(_data.name); }                \
    void setPath(const std::string& path) { _data.path = path; }                    \
    const std::string& getPath() const { return _data.path; }

#define GENERATE_RESOURCEABLE_EXTEND(className, dataMembers, baseClassName)         \
protected:                                                                          \
    struct Data {                                                                   \
        baseClassName::Data& base;                                                  \
        dataMembers                                                                 \
    };                                                                              \
    Data _data{baseClassName::_data};                                               \
                                                                                    \
public:                                                                             \
    static constexpr std::string_view type = #className;                            \
    static constexpr uint64_t typeHash = hash_string(#className);                   \


#endif // C_RESOURCEABLE_HPP