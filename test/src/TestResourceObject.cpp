#include "TestResourceObject.hpp"

#include <cstddef>
#include <iostream>

#include <glaze/json/write.hpp>


class TestResourceObject::Impl {
	public:
		Impl() = default;
	
		void setId(size_t id);
		void setStr(std::string_view newStr);
		[[nodiscard]] auto getId() const -> size_t;
		[[nodiscard]] auto getStr() const -> const std::string&;
	private:
		size_t id = 0;
		std::string str = "";
		friend struct glz::meta<TestResourceObject::Impl>;
	};
template<>
struct glz::meta<TestResourceObject::Impl> {
    static constexpr auto value = glz::object(
        &TestResourceObject::Impl::id,
        &TestResourceObject::Impl::str
    );
};

template<>
struct glz::meta<TestResourceObject> {
    static constexpr auto value = glz::object(
            &TestResourceObject::_resourceBase,
            &TestResourceObject::_impl
        );
};

void TestResourceObject::Impl::setId(size_t id) {
    this->id = id;
}

void TestResourceObject::Impl::setStr(std::string_view newStr) {
    str = newStr;
}

auto TestResourceObject::Impl::getId() const -> size_t {
    return id;
}

auto TestResourceObject::Impl::getStr() const -> const std::string& {
    return str;
}

TestResourceObject::TestResourceObject() 
    : _resourceBase(dynamic_cast<ResourceBase<TestResourceObject>*>(this))
    , _impl(std::make_unique<TestResourceObject::Impl>())
{
}

TestResourceObject::~TestResourceObject() {
    std::cout << "~TestResourceObject()\n";
}

auto TestResourceObject::deserialize(std::string_view json) -> TestResourceObject* {
    TestResourceObject* obj = new TestResourceObject;
    obj->_deserialize(json);
    return obj;
}

auto TestResourceObject::serialize(TestResourceObject* obj) -> std::string {
    return obj->_serialize();
}

auto TestResourceObject::_serialize() -> std::string {
    std::string s{};
    glz::write_json(*this, s);
    return s;
}
auto TestResourceObject::_deserialize(std::string_view json) -> TestResourceObject* {
    glz::read_json(*this, json);
    return this;
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