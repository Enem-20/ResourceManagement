# SPDX-License-Identifier: MIT
# Copyright (c) 2026 Sergey Zamaro

project(ResourceManagementTest)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

set(glaze_ENABLE_REFLECTION26 ON)

add_executable(ResourceManagementTest)
target_sources(ResourceManagementTest PRIVATE ${HEADERS} ${SOURCES})

target_compile_options(ResourceManagementTest PRIVATE -std=c++26 -freflection)

target_link_libraries(ResourceManagementTest PRIVATE ResourceManagement)

target_include_directories(ResourceManagementTest PRIVATE ../external/glaze/include)