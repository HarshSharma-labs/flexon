cmake_minimum_required(VERSION 3.10)
project(flexon)

set(OpenGL_GL_PREFERENCE LEGACY)
# Find required packages
find_package(LibXml2 REQUIRED)
find_package(Curses REQUIRED)
find_package(OpenGL REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)


# Set CMake export compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(GENERATED_HEADER ${PROJECT_SOURCE_DIR}/generated/flexontoken.hpp)
set(GENERATED_CPP ${PROJECT_SOURCE_DIR}/generated/flexontoken.cpp)
#set(CMAKE_BUILD_TYPE Debug)

set(ENGINE_NAME flexon)



# Include necessary directories
include_directories(
    "${PROJECT_SOURCE_DIR}/generated/*"
    "${PROJECT_SOURCE_DIR}/include/*"
    "${PROJECT_SOURCE_DIR}/include/flexon/*"
    "${PROJECT_SOURCE_DIR}/src/*"
    "${PROJECT_SOURCE_DIR}/renderer/GUI/*"
    "${PROJECT_SOURCE_DIR}/renderer/*"
    "${PROJECT_SOURCE_DIR}/renderer/TUI/*" 
     ${LIBXML2_INCLUDE_DIR}
     ${CURSES_INCLUDE_DIR}
     ${GLFW_INCLUDE_DIR}
)

# Find source files
file(GLOB SRCS_FILES 
      "${PROJECT_SOURCE_DIR}/src/*"
      "${PROJECT_SOURCE_DIR}/include/*"
      "${PROJECT_SOURCE_DIR}/include/flexon/*"
      "${PROJECT_SOURCE_DIR}/renderer/GUI/*"
      "${PROJECT_SOURCE_DIR}/renderer/*"
      "${PROJECT_SOURCE_DIR}/renderer/TUI/*"


)

# Add executable
add_executable(${ENGINE_NAME} ${SRCS_FILES})

# Link libraries to the executable
target_link_libraries(${ENGINE_NAME} ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES} ${LIBXML2_LIBRARIES} ${CURSES_LIBRARIES})


