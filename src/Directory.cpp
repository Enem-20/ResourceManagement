#include "Directory.hpp"

Directory::Directory(const std::string& path) 
    : _path(path)
    , _stdPath(path)
{
    
}

Directory* Directory::getParent() const {
    return _parent;
}

Directory* Directory::getChild(size_t childIndex) const {
    if(childIndex < _children.size()) {
        return _children[childIndex];
    }
    return nullptr;
}

std::vector<Directory*>& Directory::getChildren() {
    return _children;
}