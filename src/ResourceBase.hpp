#ifndef C_RESOURCE_BASE_HPP
#define C_RESOURCE_BASE_HPP



#include <string>
#include <cstdint>
#include <string_view>

#include <glaze/glaze.hpp>

#include "ResourceManager.hpp"

// === 1. FNV‑1a 64‑bit for std::string_view ===
static constexpr std::uint64_t FNV_OFFSET_BASIS = 0xcbf29ce484222325ULL;
static constexpr std::uint64_t FNV_PRIME        = 0x100000001b3ULL;

static constexpr std::uint64_t fnv1a(std::string_view str) {
    std::uint64_t hash = FNV_OFFSET_BASIS;
    for (unsigned char c : str) {
        hash = (hash ^ c) * FNV_PRIME;
    }
    return hash;
}

// === 2.  __PRETTY_FUNCTION__ / __FUNCSIG__ ===
template <typename T>
static constexpr std::string_view type_name_impl() {
#ifdef __clang__  // или GCC
    constexpr std::string_view p = __PRETTY_FUNCTION__;
    constexpr std::string_view key = "type_name_impl() [T = ";
    const auto start = p.find(key) + key.size();
    const auto end   = p.find(']', start);
    return p.substr(start, end - start);
#elif defined(__GNUC__)
    constexpr std::string_view p = __PRETTY_FUNCTION__;
    constexpr std::string_view key = "type_name_impl() [with T = ";
    const auto start = p.find(key) + key.size();
    const auto end   = p.find(']', start);
    return p.substr(start, end - start);
#elif defined(_MSC_VER)
    constexpr std::string_view p = __FUNCSIG__;
    constexpr std::string_view key = "type_name_impl<";
    const auto start = p.find(key) + key.size();
    const auto end   = p.find(">(void)", start);
    return p.substr(start, end - start);
#else
    return "<unknown>";
#endif
}

template <typename T>
struct TypeName {
    static constexpr std::string_view name = type_name_impl<T>();
};

template <typename T>
constexpr std::string_view TypeName<T>::name;

template<class Derived>
class ResourceBase {
public:
    static constexpr std::string_view type = TypeName<Derived>::name;
    static constexpr std::uint64_t type_hash = fnv1a(type);

    static Derived* deserialize(std::string_view json);
	static std::string serialize(Derived* obj);

    ResourceBase();

    virtual void initialize(std::string_view name);
    virtual std::string _serialize();
    virtual Derived* _deserialize(std::string_view json);

    std::string_view getName() const;
    uint64_t getNameHash() const;
    
private:
    std::string _name;
    uint64_t _nameHash;
    
};

template<class Derived>
Derived* ResourceBase<Derived>::deserialize(std::string_view json) {
    Derived* obj = new Derived();
    obj->_deserialize(json);
    return obj;
}

template<class Derived>
std::string ResourceBase<Derived>::serialize(Derived* obj) {
    return obj->_serialize();
}

template<class Derived>
ResourceBase<Derived>::ResourceBase() {
    Derived* derived = dynamic_cast<Derived*>(this);
    
    ResourceManager::getInstance()->addResource<Derived>(derived);
}

template <class Derived>
void ResourceBase<Derived>::initialize(std::string_view name) {
    _name = name;
    _nameHash = fnv1a(_name);
}

template<class Derived>
std::string ResourceBase<Derived>::_serialize() {
    return glz::write_json(*dynamic_cast<Derived*>(this)).value_or("error");
}

template<class Derived>
Derived* ResourceBase<Derived>::_deserialize(std::string_view json) {
    Derived* obj = new Derived();
    obj->_deserialize(json);
    return obj;
}

template <class Derived>
std::string_view ResourceBase<Derived>::getName() const {
    return _name;
}

template<class Derived>
uint64_t ResourceBase<Derived>::getNameHash() const {
    return _nameHash;
}

#endif