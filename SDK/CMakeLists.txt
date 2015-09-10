

cmake_minimum_required(VERSION 2.8)

# Main project name is ARMaker
project(OpenGL_CS557)


set(CFILE_PATH  "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/" ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

message(STATUS "Source directory ${CMAKE_SOURCE_DIR}")
message(STATUS "SDK directory ${CMAKE_SOURCE_DIR}/")

set(OUTPUT_LIBDIR ${CMAKE_SOURCE_DIR}/lib)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)


# add and compile GML
add_subdirectory(glm-0.9.6.3) 
add_subdirectory(glfw-3.1.1)
add_subdirectory(glew-1.13.0)

# Copy files from source directory to destination directory, substituting any
# variables.  Create destination directory if it does not exist.

macro(configure_files srcDir destDir)
    message(STATUS "Configuring directory ${destDir}")
    make_directory(${destDir})

    file(GLOB templateFiles RELATIVE ${srcDir} ${srcDir}/*.hpp)
    foreach(templateFile ${templateFiles})
        set(srcTemplatePath ${srcDir}/${templateFile})
        if(NOT IS_DIRECTORY ${srcTemplatePath})
            # message(STATUS "Configuring file ${templateFile}")
            configure_file(
                    ${srcTemplatePath}
                    ${destDir}/${templateFile}
                    @ONLY)
        endif(NOT IS_DIRECTORY ${srcTemplatePath})
    endforeach(templateFile)
endmacro(configure_files)

## Copies all the files
macro(configure_afiles srcDir destDir)
    message(STATUS "Configuring directory ${destDir}")
    make_directory(${destDir})

    file(GLOB templateFiles RELATIVE ${srcDir} ${srcDir}/*)
    foreach(templateFile ${templateFiles})
        set(srcTemplatePath ${srcDir}/${templateFile})
        if(NOT IS_DIRECTORY ${srcTemplatePath})
            # message(STATUS "Configuring file ${templateFile}")
            configure_file(
                    ${srcTemplatePath}
                    ${destDir}/${templateFile}
                    @ONLY)
        endif(NOT IS_DIRECTORY ${srcTemplatePath})
    endforeach(templateFile)
endmacro(configure_afiles)

# configure glm code
configure_files("${CMAKE_SOURCE_DIR}/glm-0.9.6.3/glm" "${CMAKE_SOURCE_DIR}/include/glm")
configure_afiles("${CMAKE_SOURCE_DIR}/glm-0.9.6.3/glm/gtx" "${CMAKE_SOURCE_DIR}/include/glm/gtx")
configure_afiles("${CMAKE_SOURCE_DIR}/glm-0.9.6.3/glm/gtc" "${CMAKE_SOURCE_DIR}/include/glm/gtc")
configure_afiles("${CMAKE_SOURCE_DIR}/glm-0.9.6.3/glm/detail" "${CMAKE_SOURCE_DIR}/include/glm/detail")

# configure glew
configure_afiles("${CMAKE_SOURCE_DIR}/glew-1.13.0/include/GL" "${CMAKE_SOURCE_DIR}/include/GL")
#configure_afiles("${CMAKE_SOURCE_DIR}/glew-1.13.0/bin" "${CMAKE_SOURCE_DIR}/bin")
#configure_afiles("${CMAKE_SOURCE_DIR}/glew-1.13.0/bin/Release/x64" "${CMAKE_SOURCE_DIR}/bin")
#configure_afiles("${CMAKE_SOURCE_DIR}/glew-1.13.0/lib/Release/x64" "${CMAKE_SOURCE_DIR}/lib")

# configure glfw
configure_afiles("${CMAKE_SOURCE_DIR}/glfw-3.1.1/include/GLFW" "${CMAKE_SOURCE_DIR}/include/GLFW")