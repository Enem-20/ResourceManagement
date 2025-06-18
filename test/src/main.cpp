#include "iostream"

#include <ResourceManager.hpp>

#include "TestResourceObject.hpp"

int main(int argc, const char** argv) {
	auto* testObject = new TestResourceObject();
	testObject->initialize("testObject");
	testObject->setId(15);
	testObject->setStr("check");
	std::cout << "serialized: " << testObject->_serialize() << '\n';
	testObject->setId(20);
	testObject->setStr("mda");
	std::cout << "serialized: " << testObject->_serialize() << '\n';

	ResourceManager::init(std::filesystem::absolute(argv[0]).parent_path().string());
	ResourceManager::getInstance()->saveResource(testObject, "test.json");

	return 0;
}