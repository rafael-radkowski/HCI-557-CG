#pragma once


// stl include
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <experimental\filesystem>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// local
#include "LoadBitMapFile.h"


/*!
Load two bitmap images as textures from files.
@param path_and_file_texture_1 - path and file of the first image.
@param path_and_file_texture_1 - path and file of the second image.
@return int - the texture id when the texture was sucessfully loaded.
*/
inline int LoadAndCreateTexture(string path_and_file_texture_1)
{
	unsigned int texture_1;
	unsigned int channels1;
	unsigned int width1;
	unsigned int height1;

	unsigned char* data1 = LoadBitMapFile(path_and_file_texture_1, channels1, width1, height1);

	if (data1 == NULL)return -1;


	//**********************************************************************************************
	// Texture generation - background

	// Activate a texture unit
	glActiveTexture(GL_TEXTURE0);

	// Generate a texture, this function allocates the memory and
	// associates the texture with a variable.
	glGenTextures(1, &texture_1);

	// Set a texture as active texture.
	glBindTexture(GL_TEXTURE_2D, texture_1);

	// Change the parameters of your texture units.
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	// Create a texture and load it to your graphics hardware. This texture is automatically associated
	// with texture 0 and the textuer variable "texture" / the active texture.
	if (channels1 == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_BGR, GL_UNSIGNED_BYTE, data1);
	else if (channels1 == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width1, height1, 0, GL_BGRA, GL_UNSIGNED_BYTE, data1);

	//**********************************************************************************************
	// Create a midmap texture pyramid and load it to the graphics hardware.
	// Note, the MIN and MAG filter must be set to one of the available midmap filters.
	//gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,GL_RGB, GL_UNSIGNED_BYTE, data );

	// Delete your loaded data
	free(data1);



	// bind to texture unit
	glUniform1i(texture_1, 0);



	// Return the texture.
	return texture_1;


}