/**
 * @file   ResourceManager.hpp
 * @brief  Header for ResourceManager API.
 *
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Sergey Zamaro
 */

#ifndef C_RESOURCE_MANAGER_HPP
#define C_RESOURCE_MANAGER_HPP

#include <functional>
#include <iostream>
#include <algorithm>

#include <tsl/hopscotch_map.h>

#include "Resource.hpp"
#include "Resourceable.hpp"
#include "ResourceManagementEXPORT.hpp"

class RESOURCE_MANAGEMENT_EXPORT ResourceManager {
    class Impl;
    Impl* _impl;
    
    ResourceManager();
public:
    ResourceManager(const ResourceManager&) = delete;
    auto operator=(const ResourceManager&) -> ResourceManager& = delete;
    ResourceManager(ResourceManager&&) = delete;
    auto operator=(ResourceManager&&) -> ResourceManager& = delete;

    ~ResourceManager();

    static auto getInstance() -> ResourceManager*;
#if __cpp_concepts >= 201907L
    template<is_resourceable T>
    void addResource(T* rawResource, const std::function<void(void*)>& destroyer = [](void* resource) -> void {
#else
    template<class T>
    void addResource(T* rawResource, const std::function<void(void*)>& destroyer = [](void* resource) -> void {
        static_assert(is_resourceable_v<T>, "T must be a resourceable type");
#endif
        T* corrected = reinterpret_cast<T*>(resource);
        delete corrected;
    }, const std::function<void(void*)>& saver = [](void* resource) -> void {
        T* corrected = reinterpret_cast<T*>(resource);
        ResourceManager::getInstance()->saveResource<T>(corrected, corrected->getPath());
    }) {
        rawResource->setNameRehashCallback([this, rawResource](uint64_t oldHash) -> void {
            ResourceManager::getInstance()->renameResource<T>(oldHash, rawResource->getNameHash());
        });
        void* resourceVoid = reinterpret_cast<void*>(rawResource);
        addResourcePrivate(resourceVoid, T::typeHash, rawResource->getNameHash(), rawResource->getPath(), destroyer, saver);
    }
#if __cpp_concepts >= 201907L
    template<is_resourceable T>
    void removeResource(uint64_t name) {
#else
    template<class T>
    void removeResource(uint64_t name) {
        static_assert(is_resourceable_v<T>, "T must be a resourceable type");
#endif
        removeResourcePrivate(T::typeHash, name);
    }

#if __cpp_concepts >= 201907L
    template<is_resourceable T>
    void removeResource(uint64_t name, size_t precalculatedHashName) {
#else
    template<class T>
    void removeResource(uint64_t name, size_t precalculatedHashName) {
        static_assert(is_resourceable_v<T>, "T must be a resourceable type");
#endif
        removeResourcePrivate(T::typeHash, T::precalculatedHashType, name, precalculatedHashName);
    }

#if __cpp_concepts >= 201907L
    template<is_resourceable T>
    void removeResource(std::string_view name) {
#else
    template<class T>
    void removeResource(std::string_view name) {
        static_assert(is_resourceable_v<T>, "T must be a resourceable type");
#endif
        uint64_t hashedName = hash_string(name);
        removeResourcePrivate(T::typeHash, T::precalculatedHashType, hashedName, std::hash<uint64_t>()(hashedName));
    }

#if __cpp_concepts >= 201907L
    template<is_resourceable T>
    auto getResource(uint64_t name) -> T* {
#else
    template<class T>
    auto getResource(uint64_t name) -> T* {
        static_assert(is_resourceable_v<T>, "T must be a resourceable type");
#endif
        return reinterpret_cast<T*>(getResourcePrivate(T::typeHash, name));
    }

#if __cpp_concepts >= 201907L
    template<is_resourceable T>
    auto getResource(std::string_view name) -> T* {
#else
    template<class T>
    auto getResource(std::string_view name) -> T* {
        static_assert(is_resourceable_v<T>, "T must be a resourceable type");
#endif
        uint64_t hashedName = hash_string(name);
        return reinterpret_cast<T*>(getResourcePrivate(T::typeHash, T::precalculatedHashType, hashedName, std::hash<uint64_t>()(hashedName)));
    }

#if __cpp_concepts >= 201907L && __cpp_lib_concepts >= 202002L
    template<is_resourceable T>
    auto getResource(uint64_t nameHash, size_t precalculatedHashName) {
#else
    template<class T>
    auto getResource(uint64_t nameHash, size_t precalculatedHashName) {
        static_assert(is_resourceable_v<T>, "T must be a resourceable type");
#endif
        return reinterpret_cast<T*>(getResourcePrivate(T::typeHash, T::precalculatedHashType, nameHash, precalculatedHashName));
    }

#if __cpp_concepts >= 201907L
    template<is_resourceable T>
    auto loadResource(const std::string& path) -> T* {
#else
    template<class T>
    auto loadResource(const std::string& path) -> T* {
        static_assert(is_resourceable_v<T>, "T must be a resourceable type");
#endif
        return T::deserialize(loadResourcePrivate(path));
    }

#if __cpp_concepts >= 201907L
    template<is_resourceable T>
    void saveResource(T* resource, const std::string& path) {
#else
    template<class T>
    void saveResource(T* resource, const std::string& path) {
        static_assert(is_resourceable_v<T>, "T must be a resourceable type");
#endif
        saveResourcePrivate(resource->serialize(), path);
    }

#if __cpp_concepts >= 201907L
    template<is_resourceable T>
    void renameResource(uint64_t oldHash, uint64_t newHash) {
        
#else
    template<class T>
    void renameResource(uint64_t oldHash, uint64_t newHash) {
        static_assert(is_resourceable_v<T>, "T must be a resourceable type");
#endif
        renameResourcePrivate(T::typeHash, oldHash, newHash);
    }

#if __cpp_concepts >= 201907L
    template<is_resourceable T>
    auto getTypedResources() -> tsl::hopscotch_map<uint64_t, T*> {
#else
    template<class T>
    auto getTypedResources() -> tsl::hopscotch_map<uint64_t, T*> {
        static_assert(is_resourceable_v<T>, "T must be a resourceable type");
#endif
        tsl::hopscotch_map<uint64_t, T*> result;
        auto* typedResources = getTypedResourcesPrivate(T::typeHash);
        if(typedResources != nullptr) {
            result.reserve(typedResources->size());
            std::transform(typedResources->begin(), typedResources->end(),
                std::inserter(result, result.end()),
                [](const auto& pair) -> std::pair<const uint64_t, T*> {
                    return {pair.first, reinterpret_cast<T*>(pair.second->getResource())};
                }
            );
        }

        return result;
    }

    void unloadResources();

    void saveAllResources();
private:
    void addResourcePrivate(void* resource, uint64_t type, uint64_t name, const std::string& path,
        const std::function<void(void*)>& destroyer, const std::function<void(void*)>& saver);
    void removeResourcePrivate(uint64_t type, uint64_t name);
    void removeResourcePrivate(uint64_t type, size_t precalculatedHashType, uint64_t name, size_t precalculatedHashName);
    auto getResourcePrivate(uint64_t type, uint64_t name) -> void*;
    auto getResourcePrivate(uint64_t type, size_t precalculatedHashType, uint64_t nameHash, size_t precalculatedHashName) -> void*;
    auto loadResourcePrivate(const std::string& path) -> std::vector<std::byte>;
    void saveResourcePrivate(const std::vector<std::byte>& serialized, const std::string& path);
    void renameResourcePrivate(uint64_t typeHash, uint64_t oldHash, uint64_t newHash);
    auto getTypedResourcesPrivate(uint64_t typeHash) -> tsl::hopscotch_map<uint64_t, Resource*>*;
};

#endif // C_RESOURCE_MANAGER_HPP