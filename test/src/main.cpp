/**
 * @file   main.cpp
 * @brief  Example main source for my library.
 *
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Sergey Zamaro
 */

#include <iostream>

#include <ResourceManager.hpp>
#include <ostream>

#include "TestResourceObject.hpp"
#include "TestResourceObjectExtended.hpp"

auto main(int argc, const char** argv) -> int {
	ResourceManager* resourceManager = ResourceManager::getInstance();
	TestResourceObjectExtended* testObject = new TestResourceObjectExtended();
	testObject->setName("test");
	testObject->setPath("testObject.json");
	std::cout << "before resource added\n";
	resourceManager->addResource(testObject);
	std::cout << "resource added nameHash: " << testObject->getNameHash() << '\n';
	testObject->setName("test2");
	std::cout << "resource nameHash after setName: " << testObject->getNameHash() << '\n';
	
	auto* sameTestObject = resourceManager->getResource<TestResourceObjectExtended>(testObject->getNameHash());
	if(sameTestObject != nullptr) 
		std::cout << "serialized: " << sameTestObject->getName() << '\n';

	auto serialized = testObject->serialize();

	std::cout << "serialized: " << std::string_view(reinterpret_cast<const char*>(serialized.data()), serialized.size()) << '\n';

	auto typedResources = resourceManager->getTypedResources<TestResourceObjectExtended>();
	for(const auto& [name, resource] : typedResources) {
		std::cout << "name: " << name << " resource: " << resource->getName() << '\n';
	}

	auto preHash = std::hash<uint64_t>{}(hash_string(testObject->getName()));
	TestResourceObjectExtended* testObjectByPrecalculatedHash = 
	resourceManager->getResource<TestResourceObjectExtended>(testObject->getNameHash(), preHash);

	resourceManager->unloadResources();
	return 0; 
}