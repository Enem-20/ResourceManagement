#ifndef C_TEST_RESOURCE_OBJECT_HPP
#define C_TEST_RESOURCE_OBJECT_HPP

#include "Pimpl.hpp"
#include "ResourceManagementEXPORT.hpp"
#include <cstddef>

class RESOURCE_MANAGEMENT_EXPORT TestResourceObject : public Pimpl {
public:
	TestResourceObject();
	~TestResourceObject() override {};

	static TestResourceObject* deserialize(std::string_view json);
	static std::string serialize(TestResourceObject* obj);

	virtual void initialize(std::string_view name);
	virtual std::string _serialize();
    virtual void _deserialize(std::string_view json);

	virtual void setId(size_t id);
    virtual void setStr(std::string_view newStr);
	virtual size_t getId() const;
	virtual const std::string& getStr() const;
};



#endif