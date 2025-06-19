#ifndef C_TEST_RESOURCE_OBJECT_HPP
#define C_TEST_RESOURCE_OBJECT_HPP

#include <cstddef>
#include <string>

#include "ResourceManagementEXPORT.hpp"
#include <ResourceBase.hpp>



class TestResourceObject : public ResourceBase<TestResourceObject> {
public:
	class Impl;
	friend struct glz::meta<TestResourceObject>;
private:
	std::unique_ptr<Impl> _impl;
	ResourceBase<TestResourceObject>* _resourceBase;
public:
	TestResourceObject();
	~TestResourceObject() override;

	static auto deserialize(std::string_view json) -> TestResourceObject*;
	static auto serialize(TestResourceObject* obj) -> std::string;

    auto _serialize() -> std::string override;
    auto _deserialize(std::string_view json) -> TestResourceObject* override;

	virtual void setId(size_t id);
    virtual void setStr(std::string_view newStr);
	[[nodiscard]] virtual auto getId() const -> size_t;
	[[nodiscard]] virtual auto getStr() const -> const std::string&;
};


#endif