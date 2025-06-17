# How to Use the library
## Compiler requirements
Your compiler must support C++20 concepts. Minimum versions:
-   GCC ≥ 12
-   Clang ≥ 14   

*Note: The serialisation example uses the [Glaze](https://github.com/stephenberry/glaze) library, which requires C++23 or higher. The core ResourceManagement library itself works with C++20.*
## Fetching the Sources

You can obtain the library either as a standalone clone or as a Git submodule.

**Standalone clone:**

```bash

git clone https://github.com/Enem-20/ResourceManagement.git --recurse-submodules
```
**As a submodule:**

```bash
mkdir -p external/libs
git submodule add https://github.com/Enem-20/ResourceManagement.git external/libs/ResourceManagement
cd external/libs/ResourceManagement && git submodule update --init --recursive
```
## Linking the library with your target (CMake)
Add the library to your CMake project as shown below. This assumes you have already placed the sources in `external/libs/ResourceManagement`.

```cmake
cmake_minimum_required(VERSION 3.10)

project(projectTest)

add_subdirectory(external/libs/ResourceManagement)
add_executable(projectTest)

target_sources(projectTest PRIVATE src/main.cpp)

target_link_libraries(projectTest PRIVATE ResourceManagement)
```
## API in pratice

To make a class manageable by the ResourceManager, you must mark it as **resourceable**. This involves using the `GENERATE_RESOURCEABLE` macro and implementing two methods: `serialize()` and *static* one `deserialize()`.

### Declaring a Resourceable Class

Below is a complete example that defines a class `TestResourceObject` containing two data fields (`test` and `nested`). The macro generates all required boilerplate.

**TestResourceObject.hpp**

```cpp

#include <Resourceable.hpp>

struct NestedObject {
	uint64_t nestedTest = 1050;
};

class TestResourceObject {
GENERATE_RESOURCEABLE(TestResourceObject,
	std::string test;
	NestedObject nested;
)
public:
	TestResourceObject();
	~TestResourceObject();

	static auto deserialize(const std::string& serialized) -> TestResourceObject*;
	auto serialize() const -> std::string;
};
```
**TestResourceObject.cpp**

```cpp

#include "TestResourceObject.hpp"
#include <iostream>
#include <glaze/glaze.hpp>

TestResourceObject::TestResourceObject() {}
TestResourceObject::~TestResourceObject() {
	std::cout << "~TestResourceObject()\n";
}

auto TestResourceObject::deserialize(const std::string& serialized) -> TestResourceObject* {
	auto* resource = new TestResourceObject();
	auto err = glz::read<glz::opts{.error_on_unknown_keys = false, .error_on_missing_keys = false}>(
		resource->_data, serialized
	);
	if (err) {
		std::cout << "Error: " << glz::format_error(err, serialized) << '\n';
		delete resource;
		return nullptr;
	}
	return resource;
}

auto TestResourceObject::serialize() const -> std::string {
	std::string buffer;
	auto err = glz::write_json(_data, buffer);
	if (err) {
		std::cout << "Error: " << glz::format_error(err, buffer) << '\n';
		return "";
	}
	return buffer;
}
```
### What the macro GENERATE_RESOURCEABLE does
The macro takes two arguments:
-   **className** – the name of the class where the macro is placed.
-   **dataMembers** – a list of member variables (with their types) that you want to be serialised and tracked by the resource system.

After the macro expands, the following elements are automatically generated inside your class:
-   A nested data structure named `classNameData` (e.g., `TestResourceObjectData`). It contains:
    -   All the fields listed in `dataMembers`.
    -   Two additional required fields: `name` (std::string) and `path` (std::string).
-   A private member `_data` of that generated struct type.
-   Two static `constexpr` members:
    -   `type` – a `std::string_view` holding the class name.
    -   `typeHash` – a `uint64_t` hash computed with FNV-1a, used for fast type identification.
-   Convenience methods to access and modify the resource’s metadata:
    -   `setName`, `getName`, `getNameHash`
    -   `setPath`, `getPath`
### Implementing Serialisation

You must provide the `serialize()` and `deserialize()` methods. They define how your object is converted to/from a persistent format. In the example, we use the **Glaze** library to read and write JSON. You are free to use any other serialisation approach.

> ⚠️ Glaze requires a compiler that supports C++23. If you target an earlier standard, choose a compatible serialisation library and adjust the implementation accordingly.

### Using the ResourceManager
Once your class is ready, you can hand instances over to the `ResourceManager` singleton.
```cpp

#include <iostream>
#include <ResourceManager.hpp>
#include "TestResourceObject.hpp"

int main(int argc, const char** argv) {
	ResourceManager* resourceManager = ResourceManager::getInstance();

	auto* testObject = new TestResourceObject();
	testObject->setName("test");
	testObject->setPath("testObject.json");

	resourceManager->addResource(testObject);

	std::cout << "serialized: " << testObject->serialize() << '\n';
	resourceManager->unloadResources();

	return 0;
}
```
This example:
1.  Obtains the global `ResourceManager` instance.
2.  Creates a resource object, assigns it a name and a path.
3.  Registers it with the manager via `addResource`.
4.  Prints the JSON representation produced by `serialize()`.
5.  Calls `unloadResources()` to clean up all managed resources.
