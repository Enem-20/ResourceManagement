cmake_minimum_required(VERSION 3.21)
project(ResourceManagement)

set(RESOURCE_MANAGEMENT_TESTS ON)

if(RESOURCE_MANAGEMENT_TESTS)
    add_subdirectory(test)
endif()

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS src/*.hpp)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_subdirectory(external/hopscotch-map)
add_subdirectory(external/glaze)

add_library(ResourceManagement SHARED)
target_compile_definitions(ResourceManagement PRIVATE RESOURCE_MANAGEMENT_EXPORTS)

target_sources(ResourceManagement PRIVATE ${HEADERS} ${SOURCES})

target_link_libraries(ResourceManagement PRIVATE tsl::hopscotch_map glaze::glaze)

target_include_directories(ResourceManagement PUBLIC src)