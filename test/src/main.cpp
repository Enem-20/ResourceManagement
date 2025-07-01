#include <iostream>

#include <ResourceManager.hpp>

#include "TestResourceObject.hpp"

auto main(int argc, const char** argv) -> int {
	ResourceManager::init(argc, argv);
	ResourceManager* resourceManager = ResourceManager::getInstance();
	TestResourceObject* testObject = new TestResourceObject;
	resourceManager->addResource(testObject);
	testObject->initialize("testObject");
	std::cout << "serialized: " << testObject->_serialize() << '\n';
	std::cout << "serialized: " << testObject->_serialize() << '\n';

	
	resourceManager->writeFileString("test.json", testObject->_serialize());
	resourceManager->freeAllResources();
	return 0;
}