cmake_minimum_required(VERSION 3.10)
project(flexon)

# Find required packages
find_package(Vulkan REQUIRED)



# Set CMake export compile commands
set(CMAKE_CXX_STANDARD 20) # For C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE DEBUG)
set(ENGINE_NAME flexon)

include_directories(
    "${PROJECT_SOURCE_DIR}/flexon_modules/*.hpp" 
    "${PROJECT_SOURCE_DIR}/flexon_modules/components/*.hpp" 
    "${PROJECT_SOURCE_DIR}/flexon_modules/componentbase/*.hpp" 
    "${PROJECT_SOURCE_DIR}/flexon_modules/subsystem/*.hpp" 
    "${PROJECT_SOURCE_DIR}/flexon_modules/utilities/*.hpp" 
    "${PROJECT_SOURCE_DIR}/flexon_modules/subsystem/renderer/*.hpp" 
    "${PROJECT_SOURCE_DIR}/flexon_modules/subsystem/windowManager/*.hpp" 
    "${PROJECT_SOURCE_DIR}/flexon_modules/subsystem/windowManager/*.h" 

)

# Find source files
file(GLOB SRCS_FILES 
      "${PROJECT_SOURCE_DIR}/src/*.cpp"
      "${PROJECT_SOURCE_DIR}/app/*.cpp"
      "${PROJECT_SOURCE_DIR}/flexon_modules/*.cpp"
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
      "${PROJECT_SOURCE_DIR}/flexon_modules/subsystem/statemanager/*.cpp" 

)

add_executable(${ENGINE_NAME} ${SRCS_FILES})

# Link libraries to the executable
target_link_libraries(${ENGINE_NAME} vulkan wayland-client rt shaderc_shared pthread xkbcommon)


