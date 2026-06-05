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

#include "Resourceable.hpp"

class ResourceManager {
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
        ResourceManager::getInstance()->saveResourcePrivate(corrected->serialize(), corrected->getPath());
    }) {
        void* resource = reinterpret_cast<void*>(rawResource);
        addResourcePrivate(resource, T::typeHash, rawResource->getNameHash(), rawResource->getPath(), destroyer, saver);
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

    void unloadResources();

    void saveAllResources();
private:
    void addResourcePrivate(void* resource, uint64_t type, uint64_t name, const std::string& path,
        const std::function<void(void*)>& destroyer, const std::function<void(void*)>& saver);
    void removeResourcePrivate(uint64_t type, uint64_t name);
    auto getResourcePrivate(uint64_t type, uint64_t name) -> void*;
    auto loadResourcePrivate(const std::string& path) -> std::vector<std::byte>;
    void saveResourcePrivate(const std::vector<std::byte>& serialized, const std::string& path);
};

#endif // C_RESOURCE_MANAGER_HPP