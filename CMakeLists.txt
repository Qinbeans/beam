cmake_minimum_required(VERSION 3.15)
project(Beam VERSION 1.0.0 LANGUAGES CXX)

# Specify C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Build type configuration
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build" FORCE)
endif()

set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
  include(FetchContent)
  FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  )
  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_MakeAvailable(raylib)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
  endif()
endif()

# Source files
set(BEAM_CORE_SOURCES
    src/core/node.cpp
    src/core/event.cpp
    src/core/app.cpp
)

set(BEAM_SCENE_SOURCES
    src/scene/scene.cpp
    src/scene/scene_manager.cpp
)

set(BEAM_OBJECTS_SOURCES
    src/objects/game_object.cpp
    src/objects/text.cpp
    src/objects/button.cpp
    src/objects/sprite.cpp
    src/objects/input.cpp
    src/objects/frame.cpp
)

# Create library
add_library(beam
    ${BEAM_CORE_SOURCES}
    ${BEAM_SCENE_SOURCES}
    ${BEAM_OBJECTS_SOURCES}
)

# Include directories
target_include_directories(beam
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link dependencies
target_link_libraries(beam PUBLIC raylib)

# Platform-specific settings
if(APPLE)
    target_link_libraries(beam PRIVATE 
        "-framework IOKit"
        "-framework Cocoa"
        "-framework OpenGL"
    )
endif()

# Debug/Release configurations
if(MSVC)
    target_compile_options(beam PRIVATE 
        $<$<CONFIG:Debug>:/W4 /WX>
        $<$<CONFIG:Release>:/O2 /W4 /WX>
    )
else()
    target_compile_options(beam PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Werror
        $<$<CONFIG:Debug>:-O0 -g>
        $<$<CONFIG:Release>:-O3>
    )
endif()

# Examples
option(BEAM_BUILD_EXAMPLES "Build Beam examples" ON)
if(BEAM_BUILD_EXAMPLES)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/examples/CMakeLists.txt")
        add_subdirectory(examples)
    endif()
endif()

# Installation configuration
include(GNUInstallDirs)

# Install the library and its dependencies
install(TARGETS beam raylib glfw
    EXPORT beam-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_BINARY_DIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/raylib
)

# Install headers
install(DIRECTORY include/beam
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h"
)

# Export targets
install(EXPORT beam-targets
    FILE BeamTargets.cmake
    NAMESPACE Beam::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Beam
)

# Generate package configuration
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/BeamConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/BeamConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/BeamConfig.cmake"
    @ONLY
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/BeamConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/BeamConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Beam
)