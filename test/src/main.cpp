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

auto main(int argc, const char** argv) -> int {
	ResourceManager* resourceManager = ResourceManager::getInstance();
	TestResourceObject* testObject = new TestResourceObject();
	testObject->setName("test");
	testObject->setPath("testObject.json");
	resourceManager->addResource(testObject);


	std::cout << "serialized: " << testObject->serialize() << '\n';

	resourceManager->unloadResources();
	return 0;
}