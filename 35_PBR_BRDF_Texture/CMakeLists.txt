# Main cmake file 
#

#  cmake file 
#  34_BRDF_Example
#  
#  This example demonstrates the user of a direct light BRDF 
#  This cmake file is part of CS/ME/CPRE 557
# 
#	Rafael Radkowski
#	Iowa State University
#	1620 Howe Hall
#	Ames, IA 50011
#	rafael@iastate.edu
#----------------------------------------------------------------
# Last edits
#
# Oct 11, 2019, RR
# - added linker path to glfw 3.3
# - moved the add executable definition below the library paths definition. 
#
# Oct 21, 2019, RR
# - added policy 74

cmake_minimum_required(VERSION 2.6)

# policy for relative subfolders
cmake_policy(SET CMP0015 NEW)
cmake_policy(SET CMP0074 NEW)


# Main project name
project(HCI557_TextureLight_BRDF)


set(CFILE_PATH  "${CMAKE_SOURCE_DIR}/../SDK/cmake/")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/" ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../SDK/cmake/")

message(STATUS "Source directory ${CMAKE_SOURCE_DIR}")
#message(STATUS "SDK directory ${CMAKE_SOURCE_DIR}/../SDK")


# Find packages
FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(glew REQUIRED)
FIND_PACKAGE(glm REQUIRED)
FIND_PACKAGE(GLFW3 REQUIRED)


if (GLEW_FOUND)
message(STATUS "Found GLEW ${GLEW_INCLUDE_DIR} and ${GLEW_LIBRARIES}")
endif()

if (OPENGL_FOUND)
message(STATUS "Found OPENGL ${OPENGL_INCLUDE_DIR} and ${OPENGL_LIBRARIES}")
endif()

if (GLFW_FOUND)
message(STATUS "Found GLEW ${GLFW_INCLUDE_DIRS} and ${GLFW3_LIBRARY}")
else()
IF(WIN32)
ELSE(WIN32)
include_directories("/usr/local/Cellar/glfw/3.2.1/include")
include_directories("/usr/local/Cellar/glfw/1.11.0/include")
set( GLFW3_LIBRARY glfw)
ENDIF(WIN32)
endif()

# Include dirs
include_directories(../gl_common_ext)
include_directories(./../../gl_common_ext)
include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${GLEW_INCLUDE_DIR})
include_directories(${GLFW3_INCLUDE_DIR})
include_directories(${GLM_INCLUDE_DIR})


# Add all files to the configuration
file(GLOB HCI557_SRC
	main_pbr.cpp
	brdf_tex_shader.vs
	brdf_tex_shader.fs
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
	../gl_common_ext/ModelPlane.h
	../gl_common_ext/ModelPlane.cpp
    ../gl_common_ext/Texture2D.h
	../gl_common_ext/Texture2D.cpp
	../gl_common_ext/BMPLoader.h
	../gl_common_ext/BMPLoader.cpp
	../gl_common_ext/ModelOBJ.h
	../gl_common_ext/ModelOBJ.cpp
	../gl_common_ext/ModelSphere.h
	../gl_common_ext/ModelSphere.cpp
	../gl_common_ext/RenderToTexture.h
	../gl_common_ext/CameraControls.h
	../gl_common_ext/CameraControls.cpp
	../gl_common_ext/CommonTypes.h
	../gl_common_ext/BRDFTypes.h
	../gl_common_ext/FilesystemCheck.h
	../gl_common_ext/FileUtils.h
	../gl_common_ext/FileUtils.cpp
	../gl_common_ext/BRDFLoader.h
	../gl_common_ext/BRDFLoader.cpp
	
)


add_compile_definitions(_WITH_CAMERA)
add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)



# Add link directories
# Note required for this project
IF(WIN32)
	link_directories(${CMAKE_SOURCE_DIR}/../SDK/lib)
ELSE(WIN32)
	link_directories( /usr/local/Cellar/glfw/3.3/lib)
	link_directories(${CMAKE_SOURCE_DIR}/../SDK/lib)
	link_directories(/usr/local/lib)
	link_directories(/usr/local/Cellar/glfw3/3.0.4/lib)
	link_directories(/usr/local/Cellar/glfw3/3.0.4/lib)
	link_directories(/usr/local/Cellar/glew/1.11.0/lib)
	link_directories(/usr/local/Cellar/glfw/3.2.1/lib)
ENDIF(WIN32)


# Create an executable
add_executable(${PROJECT_NAME} ${HCI557_SRC} )



# Add libraries
target_link_libraries(${PROJECT_NAME}  ${GLEW_LIBRARIES} ${GLEW_LIBRARY} ${GLFW3_LIBRARY} ${OPENGL_LIBRARIES} ${OPENGL_LIBRARIES} )
