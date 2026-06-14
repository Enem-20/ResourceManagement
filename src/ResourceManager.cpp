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
    void addResource(void* resource, uint64_t type, uint64_t name, const std::string& path,
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

    void removeResource(uint64_t type, uint64_t name) {
        auto resourcesWithType = _resources.find(type);
        if(resourcesWithType != _resources.end()) {
            auto resource = resourcesWithType.value().find(name);
            if(resource != resourcesWithType.value().end()) {
                delete resource.value();
                resourcesWithType.value().erase(resource);
            }
        }
    }

    void removeResource(uint64_t type, size_t precalculatedHashType, uint64_t name, size_t precalculatedHashName) {
        auto resourcesWithType = _resources.find(type);
        if(resourcesWithType != _resources.end()) {
            auto resource = resourcesWithType.value().find(name, precalculatedHashName);
            if(resource != resourcesWithType.value().end()) {
                delete resource.value();
                resourcesWithType.value().erase(resource);
            }
        }
    }

    auto getResource(uint64_t type, uint64_t name) -> void* {
        auto resourcesWithType = _resources.find(type);
        if(resourcesWithType != _resources.end()) {
            auto resource = resourcesWithType.value().find(name);
            if(resource != resourcesWithType.value().end()) {
                return resource.value()->getResource();
            }
        }
        return nullptr;
    }

    auto getResource(uint64_t typeHash, size_t precalculatedHashType, uint64_t nameHash, size_t precalculatedHashName) -> void* {
        auto resourcesWithType = _resources.find(typeHash, precalculatedHashType);
        if(resourcesWithType != _resources.end()) {
            auto resource = resourcesWithType.value().find(nameHash, precalculatedHashName);
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

    auto loadFileBytes(const std::string& path) -> std::vector<std::byte> {
        std::vector<std::byte> data;
        FILE* f = std::fopen(path.c_str(), "rb");
        if (!f) return data;

        std::fseek(f, 0, SEEK_END);
        long size = std::ftell(f);
        if (size > 0) {
            std::rewind(f);
            data.resize(size);
            size_t r = std::fread(data.data(), 1, size, f);
            if (r != size) data.clear();
        }
        std::fclose(f);
        return data;
    }

    auto loadResource(const std::string& path) -> std::vector<std::byte> {
        return loadFileBytes(path);
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

    void writeBinaryFileC(const char* path, const std::vector<std::byte>& data) {
        FILE* f = std::fopen(path, "wb");
        if (!f) return;

        size_t written = std::fwrite(data.data(), 1, data.size(), f);
        std::fclose(f);

        //return written == data.size();
    }

    void saveResource(const std::vector<std::byte>& serialized, const std::string& path) {
        writeBinaryFileC(path.c_str(), serialized);
    }

    void unloadResources() {
        for(const auto& [type, resources] : _resources) {
            for(const auto& [name, resource] : resources) {
                resource->save();
                delete resource;
            }
        }
        _resources.clear();
    }

    void saveAllResources() {
        for(const auto& [type, resources] : _resources) {
            for(const auto& [name, resource] : resources) {
                resource->save();
            }
        }
    }

    void renameResource(uint64_t typeHash, uint64_t oldHash, uint64_t newHash) {
        auto resourcesWithType = _resources.find(typeHash);
        if(resourcesWithType != _resources.end()) {
            auto resource = resourcesWithType.value().find(oldHash);
            if(resource != resourcesWithType.value().end()) {
                Resource* resourceRaw = resource.value();
                resourcesWithType.value().erase(resource);
                resourcesWithType.value()[newHash] = resourceRaw;
            }
        }
    }

    auto getTypedResources(uint64_t typeHash) -> tsl::hopscotch_map<uint64_t, Resource*>* {
        auto resourcesWithType = _resources.find(typeHash);
        if(resourcesWithType != _resources.end()) {
            return &resourcesWithType.value();
        }
        return nullptr;
    }
private:
    using InnerMap = tsl::hopscotch_map<uint64_t, Resource*>;
    using OuterMap = tsl::hopscotch_map<uint64_t, InnerMap>;
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

void ResourceManager::addResourcePrivate(void* resource, uint64_t type, uint64_t name, const std::string& path,
    const std::function<void(void*)>& destroyer, const std::function<void(void*)>& saver) {
    _impl->addResource(resource, type, name, path, destroyer, saver);
}

void ResourceManager::removeResourcePrivate(uint64_t type, uint64_t name) {
    _impl->removeResource(type, name);
}

void ResourceManager::removeResourcePrivate(uint64_t type, size_t precalculatedHashType, uint64_t name, size_t precalculatedHashName) {
    _impl->removeResource(type, precalculatedHashType, name, precalculatedHashName);
}

auto ResourceManager::getResourcePrivate(uint64_t type, uint64_t name) -> void* {
    return _impl->getResource(type, name);
}

auto ResourceManager::getResourcePrivate(uint64_t type, size_t precalculatedHashType, uint64_t nameHash, size_t precalculatedHashName) -> void* {
    return _impl->getResource(type, precalculatedHashType, nameHash, precalculatedHashName);
}

auto ResourceManager::loadResourcePrivate(const std::string& path) -> std::vector<std::byte> {
    return _impl->loadResource(path);
}
void ResourceManager::saveResourcePrivate(const std::vector<std::byte>& serialized, const std::string& path) {
    _impl->saveResource(serialized, path);
}

void ResourceManager::renameResourcePrivate(uint64_t typeHash, uint64_t oldHash, uint64_t newHash) {
    _impl->renameResource(typeHash, oldHash, newHash);
}

auto ResourceManager::getTypedResourcesPrivate(uint64_t typeHash) -> tsl::hopscotch_map<uint64_t, Resource*>* {
    return _impl->getTypedResources(typeHash);
}