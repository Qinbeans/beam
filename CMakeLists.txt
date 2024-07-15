cmake_minimum_required(VERSION 3.15)
project(GameProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the build directories
set(BUILD_DIR ${CMAKE_BINARY_DIR})
set(OBJECTS_DIR ${BUILD_DIR}/objects)
set(GAME_OBJECTS_DIR ${OBJECTS_DIR}/game_objects)

# Include directories
include_directories(include)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-DRELEASE)
endif()

# Platform-specific settings
if(WIN32)
    set(TARGET main)
    set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib/win)
    set(LIBS ${LIB_DIR}/raylib.lib opengl32 gdi32 winmm shell32)
    set(CMAKE_CXX_COMPILER cl)
    file(COPY ${LIB_DIR}/raylib.dll DESTINATION ${BUILD_DIR})
elseif(APPLE)
    set(TARGET main)
    set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib/mac)
    set(LIBS ${LIB_DIR}/libraylib.a "-framework CoreVideo" "-framework IOKit" "-framework Cocoa" "-framework GLUT" "-framework OpenGL")
    set(CMAKE_CXX_COMPILER clang++)
elseif(UNIX)
    set(TARGET main)
    set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib/linux)
    set(LIBS ${LIB_DIR}/libraylib.a GL m pthread dl rt X11)
    set(CMAKE_CXX_COMPILER g++)
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${BUILD_DIR})

# Link directories
link_directories(${LIB_DIR})

# Add subdirectories
add_subdirectory(event)
add_subdirectory(game_objects)

# Collect all object files from the subdirectories
set(OBJECT_FILES $<TARGET_OBJECTS:event> $<TARGET_OBJECTS:game_objects>)

# Main executable
add_executable(${TARGET} main.cpp ${OBJECT_FILES})

# Link libraries
target_link_libraries(${TARGET} ${LIBS})

# Output directories
set_target_properties(${TARGET} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR}
    OUTPUT_NAME ${TARGET}
)
