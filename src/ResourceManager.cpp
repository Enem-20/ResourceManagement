#include "ResourceManager.hpp"

#include <exception>
#include <fstream>
#include <iostream>

ResourceManager* ResourceManager::instance;

void ResourceManager::init(const std::string& path) {
	if(!instance)
		instance = new ResourceManager(path);
}

ResourceManager* ResourceManager::getInstance() {
	return instance;
}

void ResourceManager::scan(const std::string& path) {

}

std::string getFileString(const std::string& path) {
	std::string result = "";

	try {
		std::ifstream f(path);
		std::stringstream ss;
		ss << f.rdbuf();
		result = ss.str();
	}
	catch(std::exception& ex) {
		std::cerr << "can't read " << path << " file. error: " << ex.what();
	}

	return result;
}

ResourceManager::ResourceManager(const std::string& initialDir) 
	: currentDir(initialDir)
{

}