cmake_minimum_required(VERSION 3.21)
project(ResourceManagement)

set(RESOURCE_MANAGEMENT_TESTS OFF)

if(RESOURCE_MANAGEMENT_TESTS)
    add_subdirectory(test)
endif()

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/hopscotch-map)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/glaze)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/Pimpl)

add_library(ResourceManagement SHARED)

target_compile_definitions(ResourceManagement PRIVATE RESOURCE_MANAGEMENT_EXPORTS)

target_sources(ResourceManagement PRIVATE ${HEADERS} ${SOURCES})

target_link_libraries(ResourceManagement PUBLIC tsl::hopscotch_map glaze::glaze Pimpl)

target_include_directories(ResourceManagement PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)