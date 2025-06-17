#ifndef C_DIRECTORY_HPP
#define C_DIRECTORY_HPP

#include <cstddef>
#include <string>
#include <filesystem>
#include <vector>

#include "ResourceManagementEXPORT.hpp"

class RESOURCE_MANAGEMENT_EXPORT Directory {
public:
    Directory(const std::string& path);
    [[nodiscard]] auto getParent() const -> Directory*;
    [[nodiscard]] auto getChild(size_t childIndex) const -> Directory*;
    auto getChildren() -> std::vector<Directory*>&;
private:
    Directory* _parent;
    std::vector<Directory*> _children;
    std::string _path;
    std::filesystem::path _stdPath;
};

#endif