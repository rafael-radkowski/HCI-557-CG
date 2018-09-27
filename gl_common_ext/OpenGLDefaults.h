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




using namespace std;


namespace cs557
{


	/*!
	This function initializes glew
	*/
	bool initGlew(void);


	/*!
	Return the glsl major / minor version.
	*/
	int GLSLMajor(void);
	int GLSLMinor(void);



}// namespace cs557