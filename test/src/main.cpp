#include <iostream>

#include <ResourceManager.hpp>

#include "TestResourceObject.hpp"

auto main(int argc, const char** argv) -> int {
	ResourceManager::init(argc, argv);
	ResourceManager* resourceManager = ResourceManager::getInstance();
	TestResourceObject* testObject = new TestResourceObject;
	resourceManager->addResource(testObject);
	testObject->initialize("testObject");
	testObject->setId(15);
	testObject->setStr("check");
	std::cout << "serialized: " << testObject->_serialize() << '\n';
	testObject->setId(19);
	testObject->setStr("tyjhtj");
	std::cout << "serialized: " << testObject->_serialize() << '\n';

	
	resourceManager->writeFileString("test.json", testObject->_serialize());
	resourceManager->freeAllResources();
	return 0;
}