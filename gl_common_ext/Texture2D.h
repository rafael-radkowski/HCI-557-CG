#pragma once


// stl include
#include <iostream>
#include <string>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

// glfw includes
#include <GLFW/glfw3.h>

// local
#include "BMPLoader.h"


using namespace std;


namespace cs557
{

    /*
    Create a 2D texture and assign it to texture unit GL_TEXTURE0 
    @param width, height - the width and the height of the texture in pixels.
    @param channels - the number of channels.
    @param texture_unit - the texture unit
    @param data - a pointer to the data. 
    @param dst_texture_id -  a pointer to return the texture identifer
    @return true if successfull
    */
    bool CreateTexture2D(int width, int height, int channels, unsigned char* data, unsigned int* dst_texture_id, int mode = GL_CLAMP_TO_BORDER, int texture_unit = GL_TEXTURE0 );


    /*
    Create a single 2D texture and assign it to texture unit GL_TEXTURE0 
    @param file_and_path - relative or absolute path pointing to the texture
    @param dst_texture_id -  a pointer to return the texture identifer
    @param texture_unit - a pointer to return the texture unit
    @return true if successfull
    */
    bool LoadAndCreateTexture2D(string file_and_path, unsigned int* dst_texture_id);



     /*
    Create a single 2D texture and assign it to texture unit GL_TEXTURE0 
    @param file_and_path - relative or absolute path pointing to the texture
    @param dst_texture_id -  a pointer to return the texture identifer
    @param texture_unit - a pointer to return the texture unit
    @param wrap_mode - the GL wrapping mode. Use GL_REPEAT, GL_CLAMP_TO_BORDER, etc.
    @Param texture_unit - set the texture unit, e.g, GL_TEXTURE0
    @return true if successfull
    */
    bool LoadAndCreateTexture2D(string file_and_path, unsigned int* dst_texture_id, int wrap_mode, int texture_unit = GL_TEXTURE0);

}
