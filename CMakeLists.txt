cmake_minimum_required(VERSION 3.10)
project(flexon)

set(OpenGL_GL_PREFERENCE LEGACY)
# Find required packages
find_package(Vulkan REQUIRED)



# Set CMake export compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_COMMAND glslc)
set(ENGINE_NAME flexon)



# Include necessary directories
include_directories(
    "${PROJECT_SOURCE_DIR}/flexon_modules/*.hpp" 
    "${PROJECT_SOURCE_DIR}/flexon_modules/components/*.hpp" 
    "${PROJECT_SOURCE_DIR}/flexon_modules/componentbase/*.hpp" 
    "${PROJECT_SOURCE_DIR}/flexon_modules/subsystem/*.hpp" 
    "${PROJECT_SOURCE_DIR}/flexon_modules/utilities/*.hpp" 
    "${PROJECT_SOURCE_DIR}/flexon_modules/subsystem/renderer/*.hpp"
    "${PROJECT_SOURCE_DIR}/flexon_modules/subsystem/windowManager/*.hpp" 
    "${PROJECT_SOURCE_DIR}/flexon_modules/subsystem/windowManager/*.h"
    "${PROJECT_SOURCE_DIR}/flexon_modules/subsystem/windowManager/*.hpp" 
    "${PROJECT_SOURCE_DIR}/flexon_modules/subsystem/windowManager/xdg/*.cpp" 


)

# Find source files
file(GLOB SRCS_FILES 
      "${PROJECT_SOURCE_DIR}/src/*.cpp"
      "${PROJECT_SOURCE_DIR}/flexon_modules/componentbase/*.cpp"
      "${PROJECT_SOURCE_DIR}/flexon_modules/subsystem/*.cpp"
      "${PROJECT_SOURCE_DIR}/flexon_modules/utilities/*.cpp" 
      "${PROJECT_SOURCE_DIR}/flexon_modules/subsystem/renderer/*.cpp" 

      "${PROJECT_SOURCE_DIR}/flexon_modules/subsystem/windowManager/*.cpp" 
      "${PROJECT_SOURCE_DIR}/flexon_modules/subsystem/windowManager/*.c" 
      "${PROJECT_SOURCE_DIR}/flexon_modules/subsystem/windowManager/keyboard/*.cpp" 
      "${PROJECT_SOURCE_DIR}/flexon_modules/subsystem/windowManager/xdg/*.cpp" 

)

set(VERTEX_COMMAND "${PROJECT_SOURCE_DIR}/flexon_modules/subsystem/renderer/shaders/vertex_shader.glsl") 
# Add executable
add_executable(${ENGINE_NAME} ${SRCS_FILES})

# Link libraries to the executable
target_link_libraries(${ENGINE_NAME} ${WAYLAND_LIBRARIES} vulkan wayland-client rt)


