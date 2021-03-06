# Requirement
cmake_minimum_required(VERSION 2.8)

# Project configuration
project(insect_tower)
file(
  GLOB_RECURSE
  MY_SOURCES
  src/*.c
) # To update depending on the ex wanted

# Linker
find_package(OpenGL REQUIRED)

include_directories( # Search directories for .h files
  src
  ${OPENGL_INCLUDE_DIR}
)

include_directories( # Search directories for .h files
    windows/dependencies/SDL2-2.0.9/include
    windows/dependencies/glew-2.1.0/include
)


# Build
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/)
add_executable(${PROJECT_NAME} ${MY_SOURCES})

    target_link_libraries(
        ${PROJECT_NAME}
        ${OPENGL_LIBRARIES}
    )  
if(WIN32)
    target_link_libraries(
        ${PROJECT_NAME}
        ${CMAKE_SOURCE_DIR}/windows/dependencies/SDL2-2.0.9/lib/x86/SDL2.lib
        ${CMAKE_SOURCE_DIR}/windows/dependencies/SDL2-2.0.9/lib/x86/SDL2main.lib
        ${CMAKE_SOURCE_DIR}/windows/dependencies/SDL2-2.0.9/lib/x86/SDL2_image.lib
        ${CMAKE_SOURCE_DIR}/windows/dependencies/SDL2-2.0.9/lib/x86/SDL2_mixer.lib
        ${CMAKE_SOURCE_DIR}/windows/dependencies/SDL2-2.0.9/lib/x86/SDL2_ttf.lib
        ${CMAKE_SOURCE_DIR}/windows/dependencies/glew-2.1.0/lib/Release/Win32/glew32.lib
    )
    file(COPY windows/dependencies/SDL2-2.0.9/lib/x86/SDL_dll/
        DESTINATION ${EXECUTABLE_OUTPUT_PATH}Debug
    )
    file(COPY windows/dependencies/glew-2.1.0/bin/Release/Win32/glew32.dll
    DESTINATION ${EXECUTABLE_OUTPUT_PATH}Debug
    )
else()
    target_link_libraries(
        ${PROJECT_NAME}
        m
        SDL
        SDL_image
        SDL_mixer
        SDL_ttf
        GL
        GLU
    )
endif()

