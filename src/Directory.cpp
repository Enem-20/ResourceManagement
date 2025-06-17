#include "Directory.hpp"

Directory::Directory(const std::string& path) 
    : _path(path)
    , _stdPath(path)
{
    
}

auto Directory::getParent() const -> Directory* {
    return _parent;
}

auto Directory::getChild(size_t childIndex) const -> Directory* {
    if(childIndex < _children.size()) {
        return _children[childIndex];
    }
    return nullptr;
}

auto Directory::getChildren() -> std::vector<Directory*>& {
    return _children;
}