#ifndef C_RESOURCE_MANAGER_HPP
#define C_RESOURCE_MANAGER_HPP

#include <cstddef>
#include <cstdint>

#include <glaze/core/opts.hpp>
#include <glaze/json/json_t.hpp>
#include <string>


#include <tsl/hopscotch_map.h>

#include "Resource.hpp"

class RESOURCE_MANAGEMENT_EXPORT ResourceManager {
public:
	ResourceManager(const ResourceManager&) = delete;
	auto operator=(const ResourceManager&) -> ResourceManager& = delete;
	ResourceManager(ResourceManager&&) = delete;
	auto operator=(ResourceManager&&) -> ResourceManager& = delete;

	static void init(const std::string& path);
	static auto getInstance() -> ResourceManager*;

	virtual ~ResourceManager() {}

	template<class T>
	auto addResource(T* rawResource) -> Resource*;
	template<class T>
	[[nodiscard]] auto getResource(size_t nameHash) const -> Resource*;
	template<class T>
	void removeResource(size_t nameHash);
	template<class T>
	auto loadResources(const std::string& path) -> std::vector<T*>;
	template<class T>
	auto loadResource(const std::string& path) -> T*;
	template<class T>
	void saveResource(T* resource, const std::string& path);
	
	void scan(const std::string& path);
	auto getFileString(const std::string& path) -> std::string;
	void writeFileString(const std::string& path, const std::string& fileString);
private:
	static ResourceManager* instance;
	std::string currentDir;
	ResourceManager(const std::string& initialDir);
	ResourceManager() = default;

	tsl::hopscotch_map<uint64_t, tsl::hopscotch_map<uint64_t, Resource*>> _resources;
};

template<class T>
auto ResourceManager::addResource(T* rawResource) -> Resource* {
	if(!rawResource)
		return nullptr;
	auto *resource = new Resource;
	resource->writeResource(rawResource, [rawResource](){
		delete rawResource;
	});
	uint64_t resourceTypeHash = T::typeHash;
	uint64_t resourceNameHash = rawResource->getNameHash();
	auto resourcesWithType = _resources.find(resourceTypeHash);
	if(resourcesWithType != _resources.end()) {
		auto resourceIt = resourcesWithType.value().find(resourceNameHash);
		if(resourceIt != resourcesWithType.value().end()) {
			return resourceIt.value();
		}
		resourcesWithType.value()[resourceNameHash] = resource;
	}
	else {
		tsl::hopscotch_map<uint64_t, Resource*> newType = {std::pair<uint64_t, Resource*>{resourceNameHash, resource}};
		_resources[resourceTypeHash] = newType;
	}

	return resource;
}

template<class T>
auto ResourceManager::getResource(uint64_t nameHash) const -> Resource* {
	uint64_t resourceTypeHash = T::typeHash;
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
	uint64_t resourceTypeHash = T::typeHash;
	auto resourcesWithType = _resources.find(resourceTypeHash);
	if(resourcesWithType != _resources.end()) {
		auto resource = resourcesWithType.value().find(nameHash);
		if(resource != resourcesWithType.value().end()) {
			delete resource.value();
		}
	}
}

template<class T>
auto ResourceManager::loadResources(const std::string& path) -> std::vector<T*> {
	std::vector<T*> result;
	std::string serializedData = getFileString(path);
	if(!serializedData.empty()) {
		auto JSONSerializedData = glz::read_json(result, serializedData);
	}

	return result;
}

template<class T>
auto ResourceManager::loadResource(const std::string& path) -> T* {
	std::string serializedData = getFileString(path);
	if(!serializedData.empty()) {
		return T::deserialize(serializedData);
	}
	return new T();
}

template<class T>
void ResourceManager::saveResource(T* resource, const std::string& path) {
	writeFileString(path, resource->_serialize());
}

#endif