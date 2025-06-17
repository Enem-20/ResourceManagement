project(ResourceManagementTest)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(ResourceManagementTest)
target_sources(ResourceManagementTest PRIVATE ${HEADERS} ${SOURCES})

target_link_libraries(ResourceManagementTest PRIVATE ResourceManagement Pimpl)