#include "TestResourceObject.hpp"

#include <cstddef>

#include <glaze/json/write.hpp>

#include "ResourceBase.hpp"


using _Impl = TestResourceObject::Pimpl::Impl;

class TestResourceObject::Pimpl::Impl : public ResourceBase<_Impl> {
public:
    Impl() = default;

    void setId(size_t id);
    void setStr(std::string_view newStr);
    [[nodiscard]] auto getId() const -> size_t;
    [[nodiscard]] auto getStr() const -> const std::string&;
private:
    size_t id;
    std::string str;
    friend struct glz::meta<_Impl>;
};

void _Impl::setId(size_t id) {
    this->id = id;
}

void _Impl::setStr(std::string_view newStr) {
    str = newStr;
}

auto _Impl::getId() const -> size_t {
    return id;
}

auto _Impl::getStr() const -> const std::string& {
    return str;
}

template <>
struct glz::meta<_Impl> {
    static constexpr auto value = object(
        &_Impl::id,
        &_Impl::str
    );
};

TestResourceObject::TestResourceObject() 
    : Pimpl(std::make_shared<_Impl>())
{

}

auto TestResourceObject::deserialize(std::string_view json) -> TestResourceObject* {
    auto* obj = new TestResourceObject;
    obj->_deserialize(json);
    return obj;
}

auto TestResourceObject::serialize(TestResourceObject* obj) -> std::string {
    return obj->_serialize();
}

void TestResourceObject::initialize(std::string_view name) {
    _impl->initialize(name);
}

auto TestResourceObject::_serialize() -> std::string {
    return _impl->_serialize();
}

void TestResourceObject::_deserialize(std::string_view json) {
    _impl.reset(ResourceBase<Impl>::deserialize(json));
}

void TestResourceObject::setId(size_t id) {
    _impl->setId(id);
}

void TestResourceObject::setStr(std::string_view newStr) {
    _impl->setStr(newStr);
}

auto TestResourceObject::getId() const -> size_t {
    return _impl->getId();
}

auto TestResourceObject::getStr() const -> const std::string& {
    return _impl->getStr();
}