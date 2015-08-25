# Install script for directory: C:/Users/rafael/Documents/Teaching/ME557_2015/src/SDK/glm-0.9.6.3/test

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "C:/Program Files/OpenGL_CS557")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Release")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("C:/Users/rafael/Documents/Teaching/ME557_2015/src/SDK/glm-0.9.6.3/test/bug/cmake_install.cmake")
  INCLUDE("C:/Users/rafael/Documents/Teaching/ME557_2015/src/SDK/glm-0.9.6.3/test/core/cmake_install.cmake")
  INCLUDE("C:/Users/rafael/Documents/Teaching/ME557_2015/src/SDK/glm-0.9.6.3/test/gtc/cmake_install.cmake")
  INCLUDE("C:/Users/rafael/Documents/Teaching/ME557_2015/src/SDK/glm-0.9.6.3/test/gtx/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

