cmake_minimum_required(VERSION 3.16)
#
# Declare the project name
project(eecs498-game-engine)

# Set the name of the executable
set(EXECUTABLE_NAME ${PROJECT_NAME})

# Create the executable
add_executable(${EXECUTABLE_NAME})

# Add your sources to the target
target_sources(${EXECUTABLE_NAME}
    PRIVATE
        src/main.cpp
        src/Engine.cpp
        src/SceneDB.cpp
)

# Include directories
add_library(include INTERFACE)
target_include_directories(include INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/include"
)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE include)

# Set C++ standard to C++20
target_compile_features(${EXECUTABLE_NAME} PUBLIC cxx_std_20)

macro(copy_resources_directory dirname)
    set(source_dir "${CMAKE_CURRENT_LIST_DIR}/src/${dirname}")
    set(dest_dir "${CMAKE_BINARY_DIR}/$<CONFIGURATION>/${dirname}")
    add_custom_command(POST_BUILD
        TARGET "${EXECUTABLE_NAME}"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${source_dir}" "${dest_dir}"
    )
endmacro()

copy_resources_directory("resources")
