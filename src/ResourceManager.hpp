#ifndef C_RESOURCE_MANAGER_HPP
#define C_RESOURCE_MANAGER_HPP

#include <cstddef>
#include <cstdint>
#include <fstream>
#include <glaze/core/opts.hpp>
#include <glaze/json/json_t.hpp>
#include <string>
#include <sstream>

#include <tsl/hopscotch_map.h>

#include "Resource.hpp"

class ResourceManager {
public:
	static void init(const std::string& path);
	static ResourceManager* getInstance();

	template<class T>
	Resource* addResource(T* rawResource);
	template<class T>
	Resource* getResource(size_t nameHash) const;
	template<class T>
	void removeResource(size_t nameHash);
	template<class T>
	std::vector<T*> loadResources(const std::string& path);
	template<class T>
	T* loadResource(const std::string& path);
	
	void scan(const std::string& path);
	std::string getFileString(const std::string& path);
private:
	static ResourceManager* instance;
	std::string currentDir;
	ResourceManager(const std::string& initialDir);
	ResourceManager() = default;
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager(ResourceManager&&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;

	tsl::hopscotch_map<uint64_t, tsl::hopscotch_map<uint64_t, Resource*>> _resources;
};

template<class T>
Resource* ResourceManager::addResource(T* rawResource) {
	if(!rawResource)
		return nullptr;
	Resource* resource = new Resource;
	resource->writeResource(rawResource, [rawResource](){
		delete rawResource;
	});
	uint64_t resourceTypeHash = T::type_hash;
	uint64_t resourceNameHash = rawResource->getNameHash();
	auto resourcesWithType = _resources.find(resourceTypeHash);
	if(resourcesWithType != _resources.end()) {
		auto resourceIt = resourcesWithType.value().find(resourceNameHash);
		if(resourceIt != resourcesWithType.value().end()) {
			return resourceIt.value();
		}
		else {
			resourcesWithType.value()[resourceNameHash] = resource;
		}
	}
	else {
		tsl::hopscotch_map<uint64_t, Resource*> newType = {std::pair<uint64_t, Resource*>{resourceNameHash, resource}};
		_resources[resourceTypeHash] = newType;
	}

	return resource;
}

template<class T>
Resource* ResourceManager::getResource(uint64_t nameHash) const {
	uint64_t resourceTypeHash = T::type_hash;
	auto resourcesWithType = _resources.find(resourceTypeHash);
	if(resourcesWithType != _resources.end()) {
		auto resource = resourcesWithType.value().find(nameHash);
		if(resource != resourcesWithType.value().end()) {
			return resource.value();
		}
	}
	return nullptr;
}

template<class T>
void ResourceManager::removeResource(uint64_t nameHash) {
	uint64_t resourceTypeHash = T::type_hash;
	auto resourcesWithType = _resources.find(resourceTypeHash);
	if(resourcesWithType != _resources.end()) {
		auto resource = resourcesWithType.value().find(nameHash);
		if(resource != resourcesWithType.value().end()) {
			delete resource.value();
		}
	}
}

template<class T>
std::vector<T*> ResourceManager::loadResources(const std::string& path) {
	std::vector<T*> result;
	std::string serializedData = getFileString(path);
	if(!serializedData.empty()) {
		auto JSONSerializedData = glz::read_json(result, serializedData);
	}

	return result;
}

template<class T>
T* ResourceManager::loadResource(const std::string& path) {
	std::string serializedData = getFileString(path);
	if(!serializedData.empty()) {
		return T::deserialize(serializedData);
	}
	return new T();
}

#endif