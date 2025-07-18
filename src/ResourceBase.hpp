#ifndef C_RESOURCE_BASE_HPP
#define C_RESOURCE_BASE_HPP



#include <string>
#include <cstdint>
#include <string_view>

#include <glaze/glaze.hpp>

#include <ResourceManagementEXPORT.hpp>

#include "ResourceManager.hpp"

// === 1. FNV‑1a 64‑bit for std::string_view ===
static constexpr std::uint64_t FNV_OFFSET_BASIS = 0xcbf29ce484222325ULL;
static constexpr std::uint64_t FNV_PRIME        = 0x100000001b3ULL;

static constexpr auto fnv1a(std::string_view str) -> std::uint64_t {
    std::uint64_t hash = FNV_OFFSET_BASIS;
    for (unsigned char c : str) {
        hash = (hash ^ c) * FNV_PRIME;
    }
    return hash;
}

// === 2.  __PRETTY_FUNCTION__ / __FUNCSIG__ ===
template <typename T>
static constexpr auto type_name_impl() -> std::string_view {
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

#define GENERATE_PIMPL(ClassName)   \
public:                             \
    class Impl;                     \
private:                            \
    std::unique_ptr<Impl> _impl;    \
friend struct glz::meta<ClassName>;  
   

template <typename T>
struct TypeName {
    static constexpr std::string_view name = type_name_impl<T>();
};

class Pimpl {
public:

private:
};

template<class Derived>
class ResourceBase {
    friend struct glz::meta<ResourceBase>;
public:
    static constexpr std::string_view type = TypeName<Derived>::name; 
    static constexpr std::uint64_t typeHash = fnv1a(type);

    static auto deserialize(std::string_view json) -> Derived*;
	static auto serialize(ResourceBase* obj) -> std::string;

    ResourceBase();
    virtual ~ResourceBase();

    virtual void initialize(std::string_view name);
    virtual std::string _serialize() const;
    virtual Derived* _deserialize(std::string_view json);

    [[nodiscard]] auto getName() const -> std::string_view;
    [[nodiscard]] auto getNameHash() const -> uint64_t;
protected:
    std::string _name;
    uint64_t _nameHash{};
};

template<class Derived>
auto ResourceBase<Derived>::deserialize(std::string_view json) -> Derived* {
    Derived* obj = new Derived;
    glz::read_json(*dynamic_cast<Derived*>(obj), json);
    return obj;
}

template<class Derived>
auto ResourceBase<Derived>::serialize(ResourceBase* obj) -> std::string {
    std::string str;
    glz::write_json(obj, str);
    return str;
}

template<class Derived>
void ResourceBase<Derived>::initialize(std::string_view name) {
    _name = name;
    _nameHash = fnv1a(name);
}

template<class Derived>
std::string ResourceBase<Derived>::_serialize() const {
    std::string str{};
    glz::write_json(this, str);
    return str;
}

template<class Derived>
Derived* ResourceBase<Derived>::_deserialize(std::string_view json) {
    Derived* obj = dynamic_cast<Derived*>(this);
    glz::read_json(obj, json);
    return obj;
}

template<class Derived> struct glz::meta<ResourceBase<Derived>> {
    static constexpr auto value = glz::object(
        &ResourceBase<Derived>::_name
    );
};

template<class Derived>
ResourceBase<Derived>::ResourceBase() {

}

template<class Derived>
ResourceBase<Derived>::~ResourceBase() {

}

template<class Derived>
auto ResourceBase<Derived>::getName() const -> std::string_view {
    return _name;
}

template<class Derived>
auto ResourceBase<Derived>::getNameHash() const -> uint64_t {
    return _nameHash;
}
#endif