# Get all immediate subdirectories
file(GLOB EXAMPLE_DIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)
set(EXAMPLE_DIRS_FILTERED "")

# Filter for only directories and exclude hidden directories
foreach(dir ${EXAMPLE_DIRS})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${dir} AND NOT dir MATCHES "^\\.")
        list(APPEND EXAMPLE_DIRS_FILTERED ${dir})
    endif()
endforeach()

# Process each example directory
foreach(example_dir ${EXAMPLE_DIRS_FILTERED})
    # Find main source file (could be main.cpp or named after directory)
    set(MAIN_SOURCE "")
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${example_dir}/main.cpp")
        set(MAIN_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/${example_dir}/main.cpp")
    elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${example_dir}/${example_dir}.cpp")
        set(MAIN_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/${example_dir}/${example_dir}.cpp")
    endif()

    # If we found a main source file, set up the example
    if(MAIN_SOURCE)
        # Find all additional source files in the example directory
        file(GLOB_RECURSE ADDITIONAL_SOURCES
            "${CMAKE_CURRENT_SOURCE_DIR}/${example_dir}/*.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/${example_dir}/*.h"
        )
        # Remove the main source from additional sources to avoid duplication
        list(REMOVE_ITEM ADDITIONAL_SOURCES ${MAIN_SOURCE})

        # Create the executable
        add_executable(${example_dir} ${MAIN_SOURCE} ${ADDITIONAL_SOURCES})

        # Set C++ standard
        set_target_properties(${example_dir} PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
        )

        # Link with beam library
        target_link_libraries(${example_dir} PRIVATE beam)

        # Include directories
        target_include_directories(${example_dir} PRIVATE
            ${CMAKE_SOURCE_DIR}/include
            ${CMAKE_CURRENT_SOURCE_DIR}/${example_dir}
        )

        # Set output directory
        set_target_properties(${example_dir} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples/${example_dir}"
        )

        # Platform-specific settings
        if(APPLE)
            target_link_libraries(${example_dir} PRIVATE
                "-framework IOKit"
                "-framework Cocoa"
                "-framework OpenGL"
            )
        endif()

        # Add debug/release configurations
        if(MSVC)
            target_compile_options(${example_dir} PRIVATE
                $<$<CONFIG:Debug>:/W4>
                $<$<CONFIG:Release>:/O2 /W4>
            )
        else()
            target_compile_options(${example_dir} PRIVATE
                -Wall
                -Wextra
                -Wpedantic
                $<$<CONFIG:Debug>:-O0 -g>
                $<$<CONFIG:Release>:-O3>
            )
        endif()

        # Copy example-specific resources if they exist
        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${example_dir}/resources")
            add_custom_command(TARGET ${example_dir} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_CURRENT_SOURCE_DIR}/${example_dir}/resources"
                "${CMAKE_BINARY_DIR}/examples/${example_dir}/resources"
            )
        endif()

        message(STATUS "Added example: ${example_dir}")
    endif()
endforeach()

# Copy global resources if they exist
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/resources")
    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/resources"
         DESTINATION "${CMAKE_BINARY_DIR}/examples")
endif()
