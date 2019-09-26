# Main cmake file 
# Simple Model

cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0015 NEW)

# Main project name is ARMaker
project(OpenGL_Transformations)


set(CFILE_PATH  "${CMAKE_SOURCE_DIR}/../SDK/cmake/")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/" ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../SDK/cmake/")

message(STATUS "Source directory ${CMAKE_SOURCE_DIR}")
message(STATUS "SDK directory ${CMAKE_SOURCE_DIR}/../SDK")


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
include_directories(${CMAKE_SOURCE_DIR}/../gl_common_ext/)

# Add all files to the configuration
file(GLOB OpenGL_Transformations_SRC
	transformation_main.cpp
    ../gl_common_ext/OpenGLDefaults.h
	../gl_common_ext/OpenGLDefaults.cpp
	../gl_common_ext/VertexBuffers.h
	../gl_common_ext/VertexBuffers.cpp
	../gl_common_ext/ShaderProgram.h
	../gl_common_ext/ShaderProgram.cpp
	../gl_common_ext/TrackballControls.h
	../gl_common_ext/TrackballControls.cpp
	../gl_common_ext/ModelCoordinateSystem.cpp
	../gl_common_ext/ModelCoordinateSystem.h
	../gl_common_ext/Window.cpp
	../gl_common_ext/Window.h
	../gl_common_ext/ModelBox.cpp
	../gl_common_ext/ModelBox.h
	../gl_common_ext/CameraControls.h
	../gl_common_ext/CameraControls.cpp
	../gl_common_ext/CommonTypes.h
)

#definition for GLM
add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)
add_compile_definitions(_WITH_CAMERA)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Create an executable
add_executable(OpenGL_Transformations ${OpenGL_Transformations_SRC})


# Add link directories
# Note required for this project
IF(WIN32)
	link_directories(${CMAKE_SOURCE_DIR}/../SDK/lib)
ELSE(WIN32)
	link_directories(${CMAKE_SOURCE_DIR}/../SDK/lib)
	link_directories(/usr/local/lib)
	link_directories(/usr/local/Cellar/glfw3/3.0.4/lib)
	link_directories(/usr/local/Cellar/glew/1.11.0/lib)
ENDIF(WIN32)


# Add libraries
target_link_libraries(OpenGL_Transformations ${GLEW_LIBRARY} ${GLFW3_LIBRARY} ${OPENGL_LIBRARIES} ${OPENGL_LIBRARIES} )
