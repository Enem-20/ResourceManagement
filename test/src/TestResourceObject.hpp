/**
 * @file   TestResourceObject.hpp
 * @brief  Example header for my library.
 *
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Sergey Zamaro
 */

#ifndef C_TEST_RESOURCE_OBJECT_HPP
#define C_TEST_RESOURCE_OBJECT_HPP

#include <Resourceable.hpp>

struct NestedObject {
	std::string nestedTest = "nestedTest";
};

class TestResourceObject {
	GENERATE_RESOURCEABLE(TestResourceObject,
		std::string test;
		NestedObject nested;
	)
public:
	TestResourceObject();
	~TestResourceObject();

	static auto deserialize(const std::string& serialized) -> TestResourceObject*;
	auto serialize() const -> std::string;
};

#endif