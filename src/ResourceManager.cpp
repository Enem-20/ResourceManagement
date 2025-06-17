#include "ResourceManager.hpp"

#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>

ResourceManager* ResourceManager::instance;

void ResourceManager::init(const std::string& path) {
	if(!instance)
		instance = new ResourceManager(path);
}

auto ResourceManager::getInstance() -> ResourceManager* {
	return instance;
}

void ResourceManager::scan(const std::string& path) {

}

auto ResourceManager::getFileString(const std::string& path) -> std::string {
	std::string result;
	result = "";
	try {
		std::ifstream file(currentDir + path);
		std::stringstream ss;
		ss << file.rdbuf();
		result = ss.str();
		file.close();
	}
	catch(std::exception& ex) {
		std::cerr << "can't read " << path << " file. error: " << ex.what();
	}

	return result;
}

void ResourceManager::writeFileString(const std::string& path, const std::string& fileString) {
	try {
		std::ofstream file(currentDir + path);
		std::stringstream ss;
		ss << fileString;
		file << ss.rdbuf();
		file.close();
	}
	catch(std::exception& ex) {
		std::cerr << "can't read " << path << " file. error: " << ex.what();
	}
}

ResourceManager::ResourceManager(const std::string& initialDir) 
	: currentDir(initialDir)
{

}