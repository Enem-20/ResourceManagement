#ifndef C_RESOURCE_MANAGER_HPP
#define C_RESOURCE_MANAGER_HPP

#include <cstddef>
#include <cstdint>

#include <glaze/core/opts.hpp>
#include <glaze/json/json_t.hpp>
#include <glaze/json/write.hpp>
#include <string>


#include <tsl/hopscotch_map.h>

#include "Resource.hpp"

template<class Derived>
class ResourceBase;

class Pimpl;

template<typename T>
using enable_if_derived_from_base = std::enable_if_t<
    std::is_base_of_v<ResourceBase<T>, T>
>;

class RESOURCE_MANAGEMENT_EXPORT ResourceManager {
public:
	ResourceManager(const ResourceManager&) = delete;
	auto operator=(const ResourceManager&) -> ResourceManager& = delete;
	ResourceManager(ResourceManager&&) = delete;
	auto operator=(ResourceManager&&) -> ResourceManager& = delete;

	static void init(int argc, const char** argv);
	static auto getInstance() -> ResourceManager*;

	virtual ~ResourceManager() {}

	template<class T, typename = enable_if_derived_from_base<T>>
	auto addResource(T* rawResource) -> Resource*;
	template<class T, typename = enable_if_derived_from_base<T>>
	[[nodiscard]] auto getResource(size_t nameHash) const -> Resource*;
	template<class T, typename = enable_if_derived_from_base<T>>
	void removeResource(size_t nameHash);
	template<class T, typename = enable_if_derived_from_base<T>>
	void freeResource(size_t nameHash);
	template<class T, typename = enable_if_derived_from_base<T>>
	auto loadResources(const std::string& path) -> std::vector<T*>;
	template<class T, typename = enable_if_derived_from_base<T>>
	auto loadResource(const std::string& path) -> T*;
	template<class T, typename = enable_if_derived_from_base<T>>
	void saveResources(std::span<T*> resources, const std::string& path);
	template<class T, typename = enable_if_derived_from_base<T>>
	void saveResource(T* resource, const std::string& path);
	
	void freeAllResources();

	void scan(const std::string& path);
	auto getFileString(const std::string& path) -> std::string;
	void writeFileString(const std::string& path, const std::string& fileString);
private:
	static ResourceManager* instance;
	std::string currentDir;
	ResourceManager(int argc, const char** argv);
	ResourceManager() = default;

	tsl::hopscotch_map<uint64_t, tsl::hopscotch_map<uint64_t, Resource*>> _resources;
};

template<class T, typename>
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

template<class T, typename>
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

template<class T, typename>
void ResourceManager::removeResource(uint64_t nameHash) {
	uint64_t resourceTypeHash = T::typeHash;
	auto resourcesWithType = _resources.find(resourceTypeHash);
	if(resourcesWithType != _resources.end()) {
		auto resource = resourcesWithType.value().find(nameHash);
		if(resource != resourcesWithType.value().end()) {
			resourcesWithType.value().erase(resource);
		}
	}
}

template<class T, typename>
void ResourceManager::freeResource(size_t nameHash) {
	uint64_t resourceTypeHash = T::typeHash;
	auto resourcesWithType = _resources.find(resourceTypeHash);
	if(resourcesWithType != _resources.end()) {
		auto resource = resourcesWithType.value().find(nameHash);
		if(resource != resourcesWithType.value().end()) {
			delete resource.value();
			resourcesWithType.value().erase(resource);
		}
	}
}

template<class T, typename>
auto ResourceManager::loadResources(const std::string& path) -> std::vector<T*> {
	std::vector<T*> result;
	std::string serializedData = getFileString(path);
	if(!serializedData.empty()) {
		glz::read_json(result, serializedData);
	}

	return result;
}

template<class T, typename>
auto ResourceManager::loadResource(const std::string& path) -> T* {
	std::string serializedData = getFileString(path);
	if(!serializedData.empty()) {
		return T::deserialize(serializedData);
	}
	return new T();
}

template<class T, typename>
void ResourceManager::saveResource(T* resource, const std::string& path) {
	writeFileString(path, resource->_serialize());
}

template<class T, typename>
void ResourceManager::saveResources(std::span<T*> resources, const std::string& path) {
	std::string s{};
	glz::write_json(resources, s);
	writeFileString(path, s);
}

#endif