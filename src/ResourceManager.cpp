/**
 * @file   ResourceManager.cpp
 * @brief  Source for ResourceManager API.
 *
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Sergey Zamaro
 */

#include "ResourceManager.hpp"

#include <cstdio>

#include <tsl/hopscotch_map.h>

#include "Resource.hpp"

class ResourceManager::Impl {
public:
    void addResource(void* resource, std::string_view type, std::string_view name, const std::string& path,
        const std::function<void(void*)>& destroyer, const std::function<void(void*)>& saver) {
        if(resource == nullptr)
            return;
        auto resourcesWithType = _resources.find(type);
        if(resourcesWithType != _resources.end()) {
            auto resourceIt = resourcesWithType.value().find(name);
            if(resourceIt != resourcesWithType.value().end()) {
                return;
            }
            auto *resourceWrapper = new Resource;
            resourceWrapper->writeResource(resource, destroyer, saver);
            resourcesWithType.value()[name] = resourceWrapper;
        }
        else {
            InnerMap newType;
            auto *resourceWrapper = new Resource;
            resourceWrapper->writeResource(resource, destroyer, saver);
            newType[name] = resourceWrapper;
            
            _resources[type] = newType;
        }
    }

    void removeResource(std::string_view type, std::string_view name) {
        auto resourcesWithType = _resources.find(type);
        if(resourcesWithType != _resources.end()) {
            auto resource = resourcesWithType.value().find(name);
            if(resource != resourcesWithType.value().end()) {
                delete resource.value();
                resourcesWithType.value().erase(resource);
            }
        }
    }

    auto getResource(std::string_view type, std::string_view name) -> void* {
        auto resourcesWithType = _resources.find(type);
        if(resourcesWithType != _resources.end()) {
            auto resource = resourcesWithType.value().find(name);
            if(resource != resourcesWithType.value().end()) {
                return resource.value()->getResource();
            }
        }
        return nullptr;
    }

    auto getFileString(const std::string& path) -> std::string {
        std::string result;
        FILE* file = std::fopen(path.c_str(), "rb");
        if (!file) {
            std::fprintf(stderr, "can't read %s file. error: failed to open\n", path.c_str());
            return result;
        }

        // Determine file size
        std::fseek(file, 0, SEEK_END);
        long size = std::ftell(file);
        std::rewind(file);

        if (size > 0) {
            result.resize(size);
            size_t bytesRead = std::fread(result.data(), 1, size, file);
            if (bytesRead != static_cast<size_t>(size)) {
                std::fprintf(stderr, "can't read %s file. error: incomplete read\n", path.c_str());
            }
        }
        std::fclose(file);
        return result;
    }

    auto loadResource(const std::string& path) -> std::string {
        return getFileString(path);
    }

    void writeFileString(const std::string& path, const std::string& fileString) {
        FILE* file = std::fopen(path.c_str(), "wb");
        if (!file) {
            std::fprintf(stderr, "can't write %s file. error: failed to open\n", path.c_str());
            return;
        }

        size_t written = std::fwrite(fileString.data(), 1, fileString.size(), file);
        if (written != fileString.size()) {
            std::fprintf(stderr, "can't write %s file. error: incomplete write\n", path.c_str());
        }
        std::fclose(file);
    }

    void saveResource(const std::string& serialized, const std::string& path) {
        writeFileString(path, serialized);
    }

    void unloadResources() {
        for(const auto& [type, resources] : _resources) {
            for(const auto& [name, resource] : resources) {
                resource->save();
                delete resource;
                _resources[type].erase(name);
            }
            _resources.erase(type);
        }
    }

    void saveAllResources() {
        for(const auto& [type, resources] : _resources) {
            for(const auto& [name, resource] : resources) {
                resource->save();
            }
        }
    }
private:
    using InnerMap = tsl::hopscotch_map<std::string_view, Resource*>;
    using OuterMap = tsl::hopscotch_map<std::string_view, InnerMap>;
    OuterMap _resources;
};



ResourceManager::ResourceManager()
    : _impl(new Impl())
{

}

ResourceManager::~ResourceManager() {
    unloadResources();
    delete _impl;
}

auto ResourceManager::getInstance() -> ResourceManager* {
    static ResourceManager instance;
    return &instance;
}

void ResourceManager::unloadResources() {
    _impl->unloadResources();
}

void ResourceManager::saveAllResources() {
    _impl->saveAllResources();
}

void ResourceManager::addResourcePrivate(void* resource, std::string_view type, std::string_view name, const std::string& path,
    const std::function<void(void*)>& destroyer, const std::function<void(void*)>& saver) {
    _impl->addResource(resource, type, name, path, destroyer, saver);
}
void ResourceManager::removeResourcePrivate(std::string_view type, std::string_view name) {
    _impl->removeResource(type, name);
}
auto ResourceManager::getResourcePrivate(std::string_view type, std::string_view name) -> void* {
    return _impl->getResource(type, name);
}
auto ResourceManager::loadResourcePrivate(const std::string& path) -> std::string {
    return _impl->loadResource(path);
}
void ResourceManager::saveResourcePrivate(const std::string& serialized, const std::string& path) {
    _impl->saveResource(serialized, path);
}
