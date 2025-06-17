/**
 * @file   Resource.hpp
 * @brief  Header for Resource API. This class is used to store resource data.
 *
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Sergey Zamaro
 */

#ifndef C_RESOURCE_HPP
#define C_RESOURCE_HPP

#include <functional>

#include "ResourceManagementEXPORT.hpp"


class RESOURCE_MANAGEMENT_EXPORT Resource {
public:
    Resource(const Resource&) = delete;
    auto operator=(const Resource&) -> Resource& = delete;
    Resource(Resource&&) = delete;
    auto operator=(Resource&&) -> Resource& = delete;
    Resource();
    ~Resource();

    template<class T>
    void writeResource(T* resource, const std::function<void(void*)>& destroyer, const std::function<void(void*)>& saver);
    auto getResource() -> void*;
    auto save() -> void;
private:
    std::function<void(void*)> _destroyer;
    std::function<void(void*)> _saver;
    void* _resource = nullptr;
};

template<class T>
void Resource::writeResource(T* resource, const std::function<void(void*)>& destroyer, const std::function<void(void*)>& saver) {
    _resource = resource;
    _destroyer = destroyer;
    _saver = saver;
}

#endif