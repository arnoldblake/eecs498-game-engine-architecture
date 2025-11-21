cmake_minimum_required(VERSION 3.16)
#
# Declare the project name
project(eecs498-game-engine)

# Set the name of the executable
set(EXECUTABLE_NAME ${PROJECT_NAME})

# set the output directory for built objects.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# Create the executable
add_executable(${EXECUTABLE_NAME})

# Add your sources to the target
target_sources(${EXECUTABLE_NAME}
    PRIVATE
        src/main.cpp
        src/Engine.cpp
        src/SceneDB.cpp
        src/TemplateDB.cpp
)

# Set C++ standard to C++20
target_compile_features(${EXECUTABLE_NAME} PUBLIC cxx_std_20)

# Include directories
add_library(include INTERFACE)
target_include_directories(include INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/include"
)

# Add SDL3 as a subdirectory
add_subdirectory(SDL EXCLUDE_FROM_ALL)

# Add SDL_ttf as a subdirectory
set(SDLTTF_VENDORED ON)
add_subdirectory(SDL_ttf EXCLUDE_FROM_ALL)

# SDL_mixer (used for playing audio)
set(SDLMIXER_MIDI_NATIVE OFF)     # disable formats we don't use to make the build faster and smaller. Also some of these don't work on all platforms so you'll need to do some experimentation.
set(SDLMIXER_GME OFF)
set(SDLMIXER_WAVPACK OFF)
set(SDLMIXER_MOD OFF)
set(SDLMIXER_MOD_XMP OFF)
set(SDLMIXER_OPUS OFF)
set(SDLMIXER_MP3_MPG123 OFF)
set(SDLMIXER_VORBIS_VORBISFILE OFF)
set(SDLMIXER_FLAC_LIBFLAC OFF)
set(SDLMIXER_VENDORED ON)   # tell SDL_mixer to build its own dependencies
add_subdirectory(SDL_mixer EXCLUDE_FROM_ALL)

# SDL_image (used for loading various image formats)
set(SDLIMAGE_VENDORED ON)
set(SDLIMAGE_AVIF OFF)	# disable formats we don't use to make the build faster and smaller.
set(SDLIMAGE_BMP OFF)
set(SDLIMAGE_JPEG OFF)
set(SDLIMAGE_WEBP OFF)
add_subdirectory(SDL_image EXCLUDE_FROM_ALL)

# Link the include directory to the executable
target_link_libraries(${EXECUTABLE_NAME} PRIVATE
    include
    SDL3_ttf::SDL3_ttf      # remove if you are not using SDL_ttf
    SDL3_mixer::SDL3_mixer  # remove if you are not using SDL_mixer
    SDL3_image::SDL3_image	# remove if you are not using SDL_image
    SDL3::SDL3
)

macro(copy_resources_directory dirname)
    set(source_dir "${CMAKE_CURRENT_LIST_DIR}/src/${dirname}")
    set(dest_dir "${CMAKE_BINARY_DIR}/$<CONFIGURATION>/${dirname}")
    add_custom_command(POST_BUILD
        TARGET "${EXECUTABLE_NAME}"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${source_dir}" "${dest_dir}"
    )
endmacro()

copy_resources_directory("resources")
