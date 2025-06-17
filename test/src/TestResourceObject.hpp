#ifndef C_TEST_RESOURCE_OBJECT_HPP
#define C_TEST_RESOURCE_OBJECT_HPP

#include <cstddef>
#include <string>

#include "ResourceManagementEXPORT.hpp"
#include <Pimpl.hpp>

class TestResourceObject : public Pimpl {
public:
	TestResourceObject();
	~TestResourceObject() override {};

	static auto deserialize(std::string_view json) -> TestResourceObject*;
	static auto serialize(TestResourceObject* obj) -> std::string;

	virtual void initialize(std::string_view name);
	virtual auto _serialize() -> std::string;
    virtual void _deserialize(std::string_view json);

	virtual void setId(size_t id);
    virtual void setStr(std::string_view newStr);
	[[nodiscard]] virtual auto getId() const -> size_t;
	[[nodiscard]] virtual auto getStr() const -> const std::string&;
};



#endif