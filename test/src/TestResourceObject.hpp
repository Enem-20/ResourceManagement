#ifndef C_TEST_RESOURCE_OBJECT_HPP
#define C_TEST_RESOURCE_OBJECT_HPP

#include <cstddef>
#include <string>

#include "ResourceManagementEXPORT.hpp"
#include <ResourceBase.hpp>



class TestResourceObject : public ResourceBase<TestResourceObject> {
public:
	friend struct glz::meta<TestResourceObject>;
private:
	ResourceBase* _resourceBase;
public:
	TestResourceObject();
	~TestResourceObject() override;
};

template<>
struct glz::meta<TestResourceObject> {
    static constexpr auto value = glz::object(
            &TestResourceObject::_resourceBase
        );
};

#endif