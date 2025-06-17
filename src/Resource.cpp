#include "Resource.hpp"

Resource::Resource() = default;

Resource::~Resource() {
    _destroyer();
}