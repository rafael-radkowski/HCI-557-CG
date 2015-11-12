# Main cmake file 
# Simple Keyframes

cmake_minimum_required(VERSION 2.6)

# Main project name is ARMaker
project(Simple_Keyframes)

set(CFILE_PATH  "${CMAKE_SOURCE_DIR}/../SDK/cmake/")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/" ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../SDK/cmake/")

message(STATUS "Source directory ${CMAKE_SOURCE_DIR}")
message(STATUS "SDK directory ${CMAKE_SOURCE_DIR}/../SDK/")

# Find packages
FIND_PACKAGE(glew REQUIRED)
FIND_PACKAGE(glm REQUIRED)
FIND_PACKAGE(glfw3 REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)


# Include dirs
include_directories(${GLEW_INCLUDE_DIR})
include_directories(${GLFW3_INCLUDE_DIR})
include_directories(${GLM_INCLUDE_DIR})
include_directories(${OPENGL_INCLUDE_DIR})

# Add all files to the configuration
file(GLOB Simple_Keyframes_SRC
    "main.cpp"
)

# Create an executable
add_executable(Simple_Keyframes ${Simple_Keyframes_SRC})


# Add link directories
# Note required for this project
# link_directories(${NON})


# Add libraries
target_link_libraries(Simple_Keyframes ${GLEW_LIBRARY} ${GLFW3_LIBRARY} ${OPENGL_LIBRARIES} ${OPENGL_LIBRARIES} )
