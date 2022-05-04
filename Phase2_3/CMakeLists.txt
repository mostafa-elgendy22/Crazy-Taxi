cmake_minimum_required(VERSION 3.0.0)   # Selects the minimum version of CMake required to run this file
project(GFX-LAB VERSION 0.1.0)          # Here we select the project name and version

# Here we select C++17 with all the standards required and all compiler-specific extensions disabled
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# These are the options we select for building GLFW as a library
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)        # Don't build Documentation
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)       # Don't build Tests
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)    # Don't build Examples
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)           # Don't build Installation Information
set(GLFW_USE_HYBRID_HPG ON CACHE BOOL "" FORCE)     # Add variables to use High Performance Graphics Card if available
add_subdirectory(vendor/glfw)                       # Build the GLFW project to use later as a library

# A variable with all the source files of GLAD
set(GLAD_SOURCE vendor/glad/src/gl.c)
# A variables with all the source files of Dear ImGui
set(IMGUI_SOURCES
        vendor/imgui/imgui.cpp
        vendor/imgui/imgui_demo.cpp
        vendor/imgui/imgui_draw.cpp
        vendor/imgui/imgui_widgets.cpp
        vendor/imgui/imgui_impl/imgui_impl_glfw.cpp
        vendor/imgui/imgui_impl/imgui_impl_opengl3.cpp
        )

# Combine all vendor source files together into a single variable
set(VENDOR_SOURCES ${GLAD_SOURCE} ${IMGUI_SOURCES})

# A variable with all our source files that are common between executable targets (examples)
set(COMMON_SOURCES
        source/common/application.hpp
        source/common/application.cpp
        source/common/input/keyboard.hpp
        source/common/input/mouse.hpp

        source/common/asset-loader.cpp
        source/common/asset-loader.hpp
        source/common/deserialize-utils.hpp
        
        source/common/shader/shader.hpp
        source/common/shader/shader.cpp

        source/common/mesh/vertex.hpp
        source/common/mesh/mesh.hpp
        source/common/mesh/mesh-utils.hpp
        source/common/mesh/mesh-utils.cpp

        source/common/texture/sampler.hpp
        source/common/texture/sampler.cpp
        source/common/texture/texture2d.hpp
        source/common/texture/texture-utils.hpp
        source/common/texture/texture-utils.cpp
        source/common/texture/screenshot.hpp
        source/common/texture/screenshot.cpp

        source/common/material/pipeline-state.hpp
        source/common/material/pipeline-state.cpp
        source/common/material/material.hpp
        source/common/material/material.cpp

        source/common/ecs/component.hpp
        source/common/ecs/transform.hpp
        source/common/ecs/transform.cpp
        source/common/ecs/entity.hpp
        source/common/ecs/entity.cpp
        source/common/ecs/world.hpp
        source/common/ecs/world.cpp

        source/common/components/camera.hpp
        source/common/components/camera.cpp
        source/common/components/mesh-renderer.hpp
        source/common/components/mesh-renderer.cpp
        source/common/components/free-camera-controller.hpp
        source/common/components/free-camera-controller.cpp
        source/common/components/movement.hpp
        source/common/components/movement.cpp
        source/common/components/component-deserializer.hpp

        source/common/systems/forward-renderer.hpp
        source/common/systems/forward-renderer.cpp
        source/common/systems/free-camera-controller.hpp
        source/common/systems/movement.hpp
)

# Define the directories in which to search for the included headers
include_directories(
        source/common
        vendor/glfw/include
        vendor/glad/include
        vendor/glm
        vendor/imgui
        vendor/utils
)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin)

set(STATES_SOURCES
        source/states/play-state.hpp
        source/states/mesh-test-state.hpp
        source/states/transform-test-state.hpp
        source/states/pipeline-test-state.hpp
        source/states/texture-test-state.hpp
        source/states/sampler-test-state.hpp
        source/states/material-test-state.hpp
        source/states/entity-test-state.hpp
        source/states/renderer-test-state.hpp
)

# For each example, we add an executable target
# Each target compiles one example source file and the common & vendor source files
# Then we link GLFW with each target
add_executable(GAME_APPLICATION source/main.cpp ${STATES_SOURCES} ${COMMON_SOURCES} ${VENDOR_SOURCES})
target_link_libraries(GAME_APPLICATION glfw)