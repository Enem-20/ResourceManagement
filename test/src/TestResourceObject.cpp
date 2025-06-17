/**
 * @file   TestResourceObject.cpp
 * @brief  Example source for my library.
 *
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Sergey Zamaro
 */

#include "TestResourceObject.hpp"

#include <iostream>
#include <meta>

#include <glaze/glaze.hpp>

TestResourceObject::TestResourceObject()
{

}

TestResourceObject::~TestResourceObject() {
    std::cout << "~TestResourceObject()\n";
}

auto TestResourceObject::deserialize(const std::string& serialized) -> TestResourceObject* {
    TestResourceObject* resource = new TestResourceObject();
    auto err = glz::read<glz::opts{.error_on_unknown_keys=false, .error_on_missing_keys=false}>(
        resource->_data, serialized
    );
    if (err) {
        std::cout << "Error: " << glz::format_error(err, serialized) << '\n';
        delete resource;
        return nullptr;
    }
    
    return resource;
}

auto TestResourceObject::serialize() const -> std::string {
    std::string buffer;
    auto err = glz::write_json(_data, buffer);
    if (err) {
        std::cout << "Error: " << glz::format_error(err, buffer) << '\n';
        return "";
    }
    return buffer;
}