/**
 * @file   Resource.cpp
 * @brief  Source for Resource API. This class is used to store resource data.
 *
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Sergey Zamaro
 */

#include "Resource.hpp"


Resource::Resource() = default;

Resource::~Resource() {
    if(_destroyer)
        _destroyer(_resource);
}

auto Resource::getResource() -> void* {
    return _resource;
}

auto Resource::save() -> void {
    if(_saver)
        _saver(_resource);
}