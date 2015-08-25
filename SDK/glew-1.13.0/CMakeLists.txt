#project(GLEW)

cmake_minimum_required(VERSION 2.8.9)

#
#--------------------------------------------------------------------
# Create generated files
#--------------------------------------------------------------------
include(CMakePackageConfigHelpers)


configure_package_config_file("${CMAKE_SOURCE_DIR}/glew-1.13.0/src/glewConfig.cmake.in"
                              "${CMAKE_SOURCE_DIR}/glewConfig.cmake"
                              INSTALL_DESTINATION "${CMAKE_SOURCE_DIR}"
                              PATH_VARS CMAKE_INSTALL_PREFIX
                              NO_CHECK_REQUIRED_COMPONENTS_MACRO)

#write_basic_package_version_file("${GLEW_BINARY_DIR}/glewConfigVersion.cmake" 
#                                 VERSION ${GLFW_VERSION_FULL}
#                                 COMPATIBILITY SameMajorVersion)



 install(FILES "${CMAKE_SOURCE_DIR}/glewConfig.cmake"
            DESTINATION lib${LIB_SUFFIX}/cmake/glew)


