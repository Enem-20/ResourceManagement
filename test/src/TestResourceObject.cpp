#include "TestResourceObject.hpp"

#include <iostream>



TestResourceObject::TestResourceObject() 
    : ResourceBase<TestResourceObject>()
    , _resourceBase(dynamic_cast<TestResourceObject*>(this))
{
}

TestResourceObject::~TestResourceObject() {
    std::cout << "~TestResourceObject()\n";
}