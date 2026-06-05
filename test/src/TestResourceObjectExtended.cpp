#include "TestResourceObjectExtended.hpp"

#include <iostream>
#include <meta>

#include <glaze/glaze.hpp>

TestResourceObjectExtended::TestResourceObjectExtended()
{

}

TestResourceObjectExtended::~TestResourceObjectExtended() {
    std::cout << "~TestResourceObjectExtended()\n";
}

auto TestResourceObjectExtended::deserialize(const std::vector<std::byte>& serialized) -> TestResourceObjectExtended* {
    TestResourceObjectExtended* resource = new TestResourceObjectExtended();
    auto err = glz::read<glz::opts{.error_on_unknown_keys=false, .error_on_missing_keys=false}>(
        resource->_data, serialized
    );
    if (err) {
        std::cout << "Error: " << glz::format_error(err, serialized) << '\n';
        delete resource;
        return nullptr;
    }
    
    return resource;
}

auto TestResourceObjectExtended::serialize() const -> std::vector<std::byte> {
    std::vector<std::byte> buffer;
    auto err = glz::write_json(_data, buffer);
    if (err) {
        std::cout << "Error: " << glz::format_error(err, buffer) << '\n';
        return {};
    }
    return buffer;
}