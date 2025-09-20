cmake_minimum_required(VERSION 3.10)
project(flexon)

# Find required packages
find_package(Vulkan REQUIRED)



# Set CMake export compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_COMMAND glslc)
set(ENGINE_NAME flexon)


# Find source files
file(GLOB SRCS_FILES 
      "${PROJECT_SOURCE_DIR}/src/*.cpp"
      "${PROJECT_SOURCE_DIR}/app/*.cpp"
      "${PROJECT_SOURCE_DIR}/flexon_modules/subsystem/*.cpp"
      "${PROJECT_SOURCE_DIR}/flexon_modules/utilities/*.cpp" 
      "${PROJECT_SOURCE_DIR}/flexon_modules/components/*.cpp" 
      "${PROJECT_SOURCE_DIR}/flexon_modules/subsystem/renderer/*.cpp" 
      "${PROJECT_SOURCE_DIR}/flexon_modules/subsystem/renderer/shaders/*.cpp"
      "${PROJECT_SOURCE_DIR}/flexon_modules/subsystem/windowManager/*.cpp" 
      "${PROJECT_SOURCE_DIR}/flexon_modules/subsystem/windowManager/*.c" 
      "${PROJECT_SOURCE_DIR}/flexon_modules/subsystem/windowManager/keyboard/*.cpp" 
      "${PROJECT_SOURCE_DIR}/flexon_modules/subsystem/windowManager/xdg/*.cpp" 
      "${PROJECT_SOURCE_DIR}/flexon_modules/subsystem/threads/*.cpp" 
      "${PROJECT_SOURCE_DIR}/flexon_modules/subsystem/layoutmanager/*.cpp" 

)

add_executable(${ENGINE_NAME} ${SRCS_FILES})

# Link libraries to the executable
target_link_libraries(${ENGINE_NAME} vulkan wayland-client rt shaderc_shared pthread xkbcommon)


