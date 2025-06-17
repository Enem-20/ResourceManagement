#include "TestResourceObject.hpp"

#include <cstddef>
#include <memory>

#include <glaze/json/write.hpp>

#include "Pimpl.hpp"
#include "ResourceBase.hpp"


using _Impl = TestResourceObject::Pimpl::Impl;

class TestResourceObject::Pimpl::Impl : public ResourceBase<_Impl> {
public:
    Impl() = default;

    void setId(size_t id);
    void setStr(std::string_view newStr);
    size_t getId() const;
    const std::string& getStr() const;
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

size_t _Impl::getId() const {
    return id;
}

const std::string& _Impl::getStr() const {
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

TestResourceObject* TestResourceObject::deserialize(std::string_view json) {
    TestResourceObject* obj = new TestResourceObject;
    obj->_deserialize(json);
    return obj;
}

std::string TestResourceObject::serialize(TestResourceObject* obj) {
    return obj->_serialize();
}

void TestResourceObject::initialize(std::string_view name) {
    _impl->initialize(name);
}

std::string TestResourceObject::_serialize() {
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

size_t TestResourceObject::getId() const {
    return _impl->getId();
}

const std::string& TestResourceObject::getStr() const {
    return _impl->getStr();
}