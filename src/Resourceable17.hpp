/**
 * @file   Resourceable17.hpp
 * @brief  Header type trait for c++17 support to check if a type is a resourceable.
 *
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Sergey Zamaro
 */

#include <type_traits>
#include <string>
#include <cstdint>

template <typename T, typename = void>
struct is_resourceable : std::false_type {};

template <typename T>
struct is_resourceable<T, std::void_t<
    decltype(T::typeHash),
    decltype(T::type),

    decltype(std::declval<T&>().getNameHash()),
    decltype(std::declval<T&>().getName()),
    decltype(std::declval<T&>().setName(std::declval<std::string>())),
    decltype(std::declval<T&>().getPath()),
    decltype(std::declval<T&>().setPath(std::declval<std::string>())),
    decltype(T::deserialize(std::declval<std::string>())),
    decltype(std::declval<T&>().serialize())
>> : std::integral_constant<bool,
    std::is_convertible<decltype(T::typeHash), uint64_t>::value &&
    std::is_convertible<decltype(T::type), std::string_view>::value &&
    std::is_convertible<decltype(std::declval<T&>().getNameHash()), uint64_t>::value &&
    std::is_convertible<decltype(std::declval<T&>().getName()), std::string_view>::value &&
    std::is_convertible<decltype(std::declval<T&>().getPath()), std::string>::value &&
    std::is_convertible<decltype(T::deserialize(std::declval<std::string>())), T*>::value &&
    std::is_convertible<decltype(std::declval<T&>().serialize()), std::string>::value
> {};

template <typename T>
inline constexpr bool is_resourceable_v = is_resourceable<T>::value;