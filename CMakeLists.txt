# SPDX-License-Identifier: MIT
# Copyright (c) 2026 Sergey Zamaro

cmake_minimum_required(VERSION 3.10)
project(ResourceManagement)

set(RESOURCE_MANAGEMENT_TESTS OFF)

#set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

if(RESOURCE_MANAGEMENT_TESTS)
    add_subdirectory(test)
endif()

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/hopscotch-map)

add_library(ResourceManagement SHARED)

target_compile_definitions(ResourceManagement PRIVATE RESOURCE_MANAGEMENT_EXPORTS)
#if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    target_compile_options(ResourceManagement PRIVATE 
        -ffunction-sections -fdata-sections -fno-exceptions -std=c++20
    )
    target_link_options(ResourceManagement PRIVATE
        -Wl,--gc-sections
        -Wl,--strip-all
    )
#endif()

target_sources(ResourceManagement PRIVATE ${HEADERS} ${SOURCES})

target_include_directories(ResourceManagement PRIVATE ./external/hopscotch-map/include)

target_include_directories(ResourceManagement PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)