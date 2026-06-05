/**
 * @file   main.cpp
 * @brief  Example main source for my library.
 *
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Sergey Zamaro
 */

#include <iostream>

#include <ResourceManager.hpp>

#include "TestResourceObject.hpp"
#include "TestResourceObjectExtended.hpp"

auto main(int argc, const char** argv) -> int {
	ResourceManager* resourceManager = ResourceManager::getInstance();
	TestResourceObjectExtended* testObject = new TestResourceObjectExtended();
	testObject->setName("test");
	testObject->setPath("testObject.json");
	resourceManager->addResource(testObject);

	auto serialized = testObject->serialize();

	std::cout << "serialized: " << std::string_view(reinterpret_cast<const char*>(serialized.data()), serialized.size()) << '\n';

	resourceManager->unloadResources();
	return 0;
}