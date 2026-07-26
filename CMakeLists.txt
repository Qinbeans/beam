cmake_minimum_required(VERSION 3.15)
project(Beam VERSION 1.0.3 LANGUAGES CXX)

# Nothing in src/, include/beam/, or python/src/bindings.cpp needs a C++20
# language feature, so stay on C++17 project-wide. In particular: under strict
# -std=c++20, GCC treats websocketpp 0.8.2's pre-C++20 constructor-name idiom
# (e.g. `basic<concurrency,names>(...)`) as an ill-formed parse error, not
# just a warning, so marking its headers SYSTEM doesn't help. Both the beam
# library and the Python bindings module (_beam) include websocketpp (the
# latter transitively, via websocket_client.h for the WebSocketClient
# binding), so both need to avoid C++20 here.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Build type configuration
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build" FORCE)
endif()

# Position-independent code for all targets (including fetched dependencies
# like raylib/glfw), so beam and its dependencies can also be linked into the
# Python extension module built below.
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(RAYLIB_VERSION 6.0)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
  # Pick the windowing backends for raylib's bundled GLFW. Each one needs its
  # own development headers, so enable every backend whose dependencies are
  # actually installed and let GLFW choose between them at runtime (it prefers
  # Wayland when WAYLAND_DISPLAY is set, and falls back to X11). Building only
  # one backend would produce binaries that cannot open a window on the other
  # kind of session.
  if (UNIX AND NOT APPLE)
    find_package(X11)
    if (X11_FOUND AND X11_Xrandr_FOUND AND X11_Xinerama_FOUND
        AND X11_Xcursor_FOUND AND X11_Xi_FOUND)
      set(BEAM_GLFW_X11 ON)
    else()
      set(BEAM_GLFW_X11 OFF)
    endif()

    set(BEAM_GLFW_WAYLAND OFF)
    find_package(PkgConfig QUIET)
    if (PkgConfig_FOUND)
      pkg_check_modules(BEAM_WAYLAND QUIET
        wayland-client>=0.2.7 wayland-cursor>=0.2.7 wayland-egl>=0.2.7 xkbcommon>=0.5.0)
      find_program(BEAM_WAYLAND_SCANNER wayland-scanner)
      if (BEAM_WAYLAND_FOUND AND BEAM_WAYLAND_SCANNER)
        set(BEAM_GLFW_WAYLAND ON)
      endif()
    endif()

    if (NOT BEAM_GLFW_X11 AND NOT BEAM_GLFW_WAYLAND)
      message(FATAL_ERROR
        "Neither the X11 nor the Wayland development packages were found. "
        "Install libX11/libXrandr/libXinerama/libXcursor/libXi development "
        "headers, or wayland-devel plus libxkbcommon-devel and wayland-scanner.")
    endif()

    message(STATUS "raylib/GLFW backends -- X11: ${BEAM_GLFW_X11}, Wayland: ${BEAM_GLFW_WAYLAND}")
    set(GLFW_BUILD_X11 ${BEAM_GLFW_X11} CACHE BOOL "" FORCE)
    set(GLFW_BUILD_WAYLAND ${BEAM_GLFW_WAYLAND} CACHE BOOL "" FORCE)
  endif()

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

# Add websocketpp library
include(FetchContent)

# Add standalone ASIO first use C++20
FetchContent_Declare(
  asio
  GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
  GIT_TAG        asio-1-28-0
  CMAKE_ARGS    -DCMAKE_CXX_STANDARD=20
)
FetchContent_MakeAvailable(asio)

FetchContent_Declare(
  websocketpp
  GIT_REPOSITORY https://github.com/zaphoyd/websocketpp.git
  GIT_TAG        0.8.2 # Using a stable release version
  CMAKE_ARGS    -DCMAKE_CXX_STANDARD=20
)
# websocketpp's own CMakeLists.txt declares a cmake_minimum_required below 3.5,
# which modern CMake refuses to configure. Raise the policy floor just for it.
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
FetchContent_MakeAvailable(websocketpp)
unset(CMAKE_POLICY_VERSION_MINIMUM)

# Source files
set(BEAM_CORE_SOURCES
    src/core/node.cpp
    src/core/event.cpp
    src/core/app.cpp
    src/core/logger.cpp
    src/core/websocket_client.cpp
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
    src/objects/tilemap.cpp
)

# Create library
add_library(beam
    ${BEAM_CORE_SOURCES}
    ${BEAM_SCENE_SOURCES}
    ${BEAM_OBJECTS_SOURCES}
)

# Set C++ standard for beam library
set_target_properties(beam PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    POSITION_INDEPENDENT_CODE ON
)

# Include directories
target_include_directories(beam
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        $<INSTALL_INTERFACE:websocketpp>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# asio/websocketpp are vendored dependencies; mark them SYSTEM so their
# C++20-incompatible warnings (e.g. template-id-cdtor) don't hit -Werror.
target_include_directories(beam SYSTEM
    PUBLIC
        $<BUILD_INTERFACE:${asio_SOURCE_DIR}/asio/include>
        $<BUILD_INTERFACE:${websocketpp_SOURCE_DIR}>
)

# Link dependencies
target_link_libraries(beam PUBLIC raylib)

# Add preprocessor definitions for websocketpp
target_compile_definitions(beam PRIVATE ASIO_STANDALONE _WEBSOCKETPP_CPP11_STRICT_)

# Platform-specific settings for websocketpp
if(WIN32)
    target_link_libraries(beam PRIVATE ws2_32 wsock32)
endif()

if(APPLE)
    target_link_libraries(beam PRIVATE
        "-framework IOKit"
        "-framework Cocoa"
        "-framework OpenGL"
    )
endif()

if(UNIX AND NOT APPLE)
    find_package(Threads REQUIRED)
    target_link_libraries(beam PRIVATE Threads::Threads)
endif()

# Debug/Release configurations
if(MSVC)
    target_compile_options(beam PRIVATE
        $<$<CONFIG:Debug>:/W4>
        $<$<CONFIG:Release>:/O2 /W4>
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

# Python bindings (nanobind)
option(BEAM_BUILD_PYTHON_BINDINGS "Build Python bindings for beam (requires nanobind)" OFF)
if(BEAM_BUILD_PYTHON_BINDINGS)
    find_package(Python 3.8 REQUIRED COMPONENTS Interpreter Development.Module)

    execute_process(
        COMMAND "${Python_EXECUTABLE}" -m nanobind --cmake_dir
        OUTPUT_STRIP_TRAILING_WHITESPACE
        OUTPUT_VARIABLE NB_DIR
        COMMAND_ERROR_IS_FATAL ANY
    )
    list(APPEND CMAKE_PREFIX_PATH "${NB_DIR}")
    find_package(nanobind CONFIG REQUIRED)

    # _beam includes websocket_client.h (for the WebSocketClient binding),
    # which pulls in websocketpp; that's incompatible with strict C++20 (see
    # the CMAKE_CXX_STANDARD comment above), so this stays on the project's
    # C++17 default rather than opting into C++20 the way it briefly did.
    nanobind_add_module(_beam python/src/bindings.cpp)
    target_link_libraries(_beam PRIVATE beam)

    install(TARGETS _beam LIBRARY DESTINATION beam)
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
)

# Install headers
install(DIRECTORY include/beam
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h"
)
install(FILES include/raylib.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
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
