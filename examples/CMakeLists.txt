# Find all example source files
file(GLOB EXAMPLE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# Create an executable for each example
foreach(example_source ${EXAMPLE_SOURCES})
    # Get the filename without extension
    get_filename_component(example_name ${example_source} NAME_WE)
    
    # Add executable
    add_executable(${example_name} ${example_source})
    
    # Link with beam library
    target_link_libraries(${example_name} PRIVATE beam)
    
    # Include directories
    target_include_directories(${example_name} PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )
    
    # Set output directory
    set_target_properties(${example_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples"
    )
    
    # Platform-specific settings
    if(APPLE)
        target_link_libraries(${example_name} PRIVATE 
            "-framework IOKit"
            "-framework Cocoa"
            "-framework OpenGL"
        )
    endif()
    
    # Add debug/release configurations
    if(MSVC)
        target_compile_options(${example_name} PRIVATE 
            $<$<CONFIG:Debug>:/W4>
            $<$<CONFIG:Release>:/O2 /W4>
        )
    else()
        target_compile_options(${example_name} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            $<$<CONFIG:Debug>:-O0 -g>
            $<$<CONFIG:Release>:-O3>
        )
    endif()
endforeach()

# Optional: Copy resources if needed
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/resources")
    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/resources" 
         DESTINATION "${CMAKE_BINARY_DIR}/examples")
endif()