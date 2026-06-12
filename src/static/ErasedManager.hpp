#include <cstddef>
#include <type_traits>

struct ErasedManager {
    void* instance = nullptr;
    void (*clear_func)(void*) = nullptr;
    size_t (*size_func)(const void*) = nullptr;
};

template<typename T, typename = void>
struct has_clear : std::false_type {};

template<typename T>
struct has_clear<T, std::void_t<decltype(std::declval<T&>().clear())>>
    : std::is_same<decltype(std::declval<T&>().clear()), void> {};

template<typename T, typename = void>
struct has_size : std::false_type {};

template<typename T>
struct has_size<T, std::void_t<decltype(std::declval<const T&>().size())>>
    : std::is_convertible<decltype(std::declval<const T&>().size()), std::size_t> {};

template<typename T>
struct is_manager : std::conjunction<has_clear<T>, has_size<T>> {};

template<typename T>
constexpr bool is_manager_v = is_manager<T>::value;