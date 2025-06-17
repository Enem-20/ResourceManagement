#ifndef C_RESOURCE_HPP
#define C_RESOURCE_HPP

#include <functional>

#include "ResourceManagementEXPORT.hpp"

class RESOURCE_MANAGEMENT_EXPORT Resource {
public:
    Resource();
    ~Resource();

    template<class T>
    void writeResource(T* resource, const std::function<void()>& destroyer);
private:
    std::function<void()> _destroyer;
    void* _resource;
};

template<class T>
void Resource::writeResource(T* resource, const std::function<void()>& destroyer) {
    _resource = reinterpret_cast<void*>(resource);
    _destroyer = destroyer;
}

#endif