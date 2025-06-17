#ifndef C_DIRECTORY_HPP
#define C_DIRECTORY_HPP

#include <cstddef>
#include <string>
#include <filesystem>
#include <vector>


class Directory {
public:
    Directory(const std::string& path);
    Directory* getParent() const;
    Directory* getChild(size_t childIndex) const;
    std::vector<Directory*>& getChildren();
private:
    Directory* _parent;
    std::vector<Directory*> _children;
    std::string _path;
    std::filesystem::path _stdPath;
};

#endif