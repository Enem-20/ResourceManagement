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

    template<is_resourceable T>
    void addResource(T* rawResource, const std::function<void(void*)>& destroyer = [](void* resource) -> void {
        T* corrected = reinterpret_cast<T*>(resource);
        delete corrected;
    }, const std::function<void(void*)>& saver = [](void* resource) -> void {
        T* corrected = reinterpret_cast<T*>(resource);
        ResourceManager::getInstance()->saveResourcePrivate(corrected->serialize(), corrected->getPath());
    }) {
        void* resource = reinterpret_cast<void*>(rawResource);
        addResourcePrivate(resource, T::type, rawResource->getName(), rawResource->getPath(), destroyer, saver);
    }

    template<is_resourceable T>
    void removeResource(std::string_view name) {
        void* rawResource = reinterpret_cast<void*>(rawResource);
        removeResourcePrivate(T::type, name);
    }

    template<is_resourceable T>
    auto getResource(std::string_view name) -> T* {
        reinterpret_cast<T*>(getResourcePrivate(T::type, name));
    }

    template<is_resourceable T>
    auto loadResource(const std::string& path) -> T* {
        return T::deserialize(loadResourcePrivate(path));
    }

    template<is_resourceable T>
    void saveResource(T* resource, const std::string& path) {
        saveResourcePrivate(resource->serialize(), path);
    }

    void unloadResources();

    void saveAllResources();
private:
    void addResourcePrivate(void* resource, std::string_view type, std::string_view name, const std::string& path,
        const std::function<void(void*)>& destroyer, const std::function<void(void*)>& saver);
    void removeResourcePrivate(std::string_view type, std::string_view name);
    auto getResourcePrivate(std::string_view type, std::string_view name) -> void*;
    auto loadResourcePrivate(const std::string& path) -> std::string;
    void saveResourcePrivate(const std::string& serialized, const std::string& path);
};

#endif // C_RESOURCE_MANAGER_HPP