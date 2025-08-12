cmake_minimum_required(VERSION 3.10)
project(flexon)

set(OpenGL_GL_PREFERENCE LEGACY)
# Find required packages
#find_package(LibXml2 REQUIRED)
#find_package(Curses REQUIRED)
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)


# Set CMake export compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

set(ENGINE_NAME flexon)



# Include necessary directories
include_directories(
    "${PROJECT_SOURCE_DIR}/flexon_modules/*.hpp" 
    "${PROJECT_SOURCE_DIR}/flexon_modules/components/*.hpp" 
    "${PROJECT_SOURCE_DIR}/flexon_modules/componentbase/*.hpp" 
    "${PROJECT_SOURCE_DIR}/flexon_modules/subsystem/*.hpp" 
    "${PROJECT_SOURCE_DIR}/flexon_modules/utilities/*.hpp" 

)

# Find source files
file(GLOB SRCS_FILES 
      "${PROJECT_SOURCE_DIR}/src/*.cpp"
      "${PROJECT_SOURCE_DIR}/flexon_modules/componentbase/*.cpp"
      "${PROJECT_SOURCE_DIR}/flexon_modules/subsystem/*.cpp"
      "${PROJECT_SOURCE_DIR}/flexon_modules/utilities/*.cpp" 

)

# Add executable
add_executable(${ENGINE_NAME} ${SRCS_FILES})

# Link libraries to the executable
target_link_libraries(${ENGINE_NAME} ${GLFW_LIBRARIES})


