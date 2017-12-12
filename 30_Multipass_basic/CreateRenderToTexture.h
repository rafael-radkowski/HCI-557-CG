/******************************
 This file demonstrate how a render to texture frame buffer objects
 can be set up and used to render a scene into a texture. 

 Rafael Radkowski
 Iowa State University
 rafael@iastate.edu
 Dec. 9, 2016

 Last edit:
 Dec. 12, 2017:
 - Added a description
***************************/
#pragma once

// stl
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <experimental\filesystem>


// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

/*
Create a frame buffer object for OpenGL
The function creates the fbo, the textures, and returns the descriptors indices for all objects.
@param texture_width, texture_height - the width and height of the texture to render the content to. 
	MUST BE POWER OF 2: 512, 1024, etc.
@param frame_buffer_object -  a variable in which this function can write the frame buffer object idx into.
@param texture_color -  a variable in which this function can write the color texture idx into.
@param texture_depth -  a variable in which this function can write the depth texture idx into.
*/
void CreateRenderToTexture(int texture_width, int texture_height, unsigned int& frame_buffer_object, unsigned int& texture_color,  unsigned int& texture_depth)
{
	// Create a frame buffer
	//glCreateFramebuffers(1, &frame_buffer_object);
	glGenFramebuffers(1, &frame_buffer_object);

	// bind the frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_object);

	// create a texture
	glGenTextures(1, &texture_color);
	glBindTexture(GL_TEXTURE_2D, texture_color);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, texture_width, texture_height);

	// create a texture for the depth buffer
	glGenTextures(1, &texture_depth);
	glBindTexture(GL_TEXTURE_2D, texture_depth);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, texture_width, texture_height);

	// attach color and depth texture to fbo
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture_color, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture_depth, 0);


	static const GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, draw_buffers);



}