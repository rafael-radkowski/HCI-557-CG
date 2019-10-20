# Locate the glew files 
#
# This module defines the following variables:
#
# GLEW_INCLUDE_DIR  where to find glm include files.
# GLEW_FOUND        true if both the GLM_INCLUDE_DIR and GLFW3_INCLUDE_DIR have been found.
# GLEW_LIBRARY      the name of the library;
#
# To help locate the library and include file, you can define a 
# variable called GLEW_ROOT which points to the root of the glfw library 
# installation.
#


# default search dirs
set( _glew_HEADER_SEARCH_DIRS 
  "/usr/include"
  "/usr/local/include"
  "C:/Program Files (x86)/glm/include" 
  "../../SDK/include"
  "../SDK/include"
  )
  
 set( _glew_LIB_SEARCH_DIRS
  "/usr/lib"
  "/usr/local/lib"
  "C:/Program Files (x86)/glfw/lib-msvc110" 
  "../../SDK/lib"
  "../SDK/lib"
  )
  

# Check environment for root search directory
if (DEFINED ENV{GLEW_ROOT})
	set( _glew_ENV_ROOT $ENV{GLEW_ROOT} )
	if( NOT GLEW_ROOT AND _glew_ENV_ROOT )
		set(GLEW_ROOT ${_glew_ENV_ROOT} )
	endif()
endif()

# Put user specified location at beginning of search
if( GLEW_ROOT )
	list( INSERT _glew_HEADER_SEARCH_DIRS 0 "${GLEW_ROOT}/include" )
	list( INSERT _glew_LIB_SEARCH_DIRS 0 "${GLEW_ROOT}/lib" )
endif()

# Search for the header 
FIND_PATH(GLEW_INCLUDE_DIR "GL/glew.h"
	PATHS ${_glew_HEADER_SEARCH_DIRS} )
	
# Search for the library
FIND_LIBRARY(GLEW_LIBRARY NAMES  glew libglew glew32 libglew32
  PATHS ${_glew_LIB_SEARCH_DIRS} )
  
  
if( NOT glew_ROOT )
	set( glew_ROOT ${GLEW_INCLUDE_DIR}/.. )
endif()
