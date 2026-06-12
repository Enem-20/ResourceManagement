#ifndef C_STATIC_RESOURCE_MANAGER_HPP
#define C_STATIC_RESOURCE_MANAGER_HPP

#include <string>
#include <functional>
#include <vector>
#include <bitset>

#include "../dynamic/Resourceable.hpp"




template<size_t BlockSize, size_t BlockCount, size_t Align = alignof(std::max_align_t)>
class StaticResourceManager {
private:
    class Impl;
    Impl* _impl;
public:
    StaticResourceManager();
    ~StaticResourceManager();

    StaticResourceManager(const StaticResourceManager&) = delete;
    auto operator=(const StaticResourceManager&) -> StaticResourceManager& = delete;
    StaticResourceManager(StaticResourceManager&&) = delete;
    auto operator=(StaticResourceManager&&) -> StaticResourceManager& = delete;

    template<class T>
    void addResource(T* rawResource, const std::function<void(void*)>& destroyer = [](void* resource) -> void {
        static_assert(is_resourceable_v<T>, "T must be a resourceable type");
        T* corrected = reinterpret_cast<T*>(resource);
        delete corrected;
    }, const std::function<void(void*)>& saver = [](void* resource) -> void {
        T* corrected = reinterpret_cast<T*>(resource);
        saveResourcePrivate(corrected->serialize(), corrected->getPath());
    }) {
        size_t size = sizeof(T);
        placeResource(reinterpret_cast<uint8_t*>(rawResource), size, T::typeHash, rawResource->getNameHash(), rawResource->getPath(), destroyer, saver);
    }

    template<class T>
    void removeResource(uint64_t name) {
        static_assert(is_resourceable_v<T>, "T must be a resourceable type");
        removeResourcePrivate(T::typeHash, name);
    }

    template<class T>
    auto getResource() -> T* {
        static_assert(is_resourceable_v<T>, "T must be a resourceable type");
        return reinterpret_cast<T*>(getResourcePrivate(T::typeHash, 0));
    }

    template<class T> 

    auto loadResource(const std::string& path) -> T* {
        static_assert(is_resourceable_v<T>, "T must be a resourceable type");
        auto serialized = loadResourcePrivate(path);
        return T::deserialize(serialized);
    }

    template<class T>
    void saveResource(T* resource, const std::string& path) {
        static_assert(is_resourceable_v<T>, "T must be a resourceable type");
        saveResourcePrivate(resource->serialize(), path);
    }

    void unloadResources();

    void saveAllResources();

    void defragment() {
        const size_t numBlocks = _usedBytes.size();

        size_t dest = 0;

        for (size_t src = 0; src < numBlocks; ++src) {
            if (_usedBytes[src] == 1) {
                if (dest != src) {
                    memcpy(_data + (dest * byteSize),
                                _data + (src * byteSize),
                                byteSize);
                    _usedBytes.set(dest);
                    _usedBytes.reset(src);
                }
                ++dest;
            }
        }
        
        for (size_t i = dest; i < numBlocks; ++i) {
            _usedBytes.reset(i);
        }

        load_offset = dest * byteSize;
    }
private:
    void placeResource(uint8_t* data, size_t size, uint64_t type, uint64_t name, const std::string& path,
        const std::function<void(uint8_t*)>& destroyer, const std::function<void(uint8_t*)>& saver)
    {
        int index = findFreeSpace(size);
        if (index == -1) {
            return;
        }

        _usedBytes[index] = 1;
        memcpy(_data + (index * size), data, size);
    }

    auto findFreeSpace(size_t size) -> int {
        int index = -1;
        for (int i = 0; i < byteSize; i++) {
            if (_usedBytes[i] == 0) {
                index = i;
                break;
            }
        }
        return index;
    }


    static constexpr size_t byteSize = BlockSize * BlockCount;
    size_t load_offset = 0;
    std::bitset<BlockCount> _usedBytes;
    alignas(Align) uint8_t _data[byteSize];
};

#endif // C_STATIC_RESOURCE_MANAGER_HPP