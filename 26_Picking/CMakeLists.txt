# Main cmake file 
# Picking with Scissor test example
# Rafael Radkowski, rafael@iastate.edu
# Iowa State University
# ME/CS/CprE 557 Computer Graphics, Fall 2015

cmake_minimum_required(VERSION 2.6)

# Main project name is ARMaker
project(HCI557_Picking)


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
include_directories(${CMAKE_SOURCE_DIR}/../gl_common/)

# Add all files to the configuration
file(GLOB HCI557_Picking_SRC
	main_picking.cpp
    ../gl_common/HCI557Common.cpp
    ../gl_common/HCI557Common.h
	../gl_common/controls.cpp
    ../gl_common/controls.h
	../gl_common/CoordSystem.cpp
    ../gl_common/CoordSystem.h
	../gl_common/GLObject.h
   	../gl_common/GLObject.cpp
    ../gl_common/Shaders.h
    ../gl_common/Shaders.cpp
    ../gl_common/HCI557Datatypes.h
    ../gl_common/GLAppearanceBase.h
    ../gl_common/GLAppearance.h
    ../gl_common/GLAppearance.cpp
	../gl_common/camera.h
	../gl_common/camera.cpp
    ../gl_common/GLObjectObj.h
    ../gl_common/GLObjectObj.cpp

)

set(HCI557_RES
	../data/shaders/multi_vertex_lights.fs
    ../data/shaders/multi_vertex_lights.vs
)


#----
# SRC Groups
source_group(Resources FILES ${HCI557_RES})


# Create an executable
add_executable(HCI557_Picking ${HCI557_Picking_SRC})


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
target_link_libraries(HCI557_Picking ${GLEW_LIBRARY} ${GLFW3_LIBRARY} ${OPENGL_LIBRARIES} ${OPENGL_LIBRARIES} )
						