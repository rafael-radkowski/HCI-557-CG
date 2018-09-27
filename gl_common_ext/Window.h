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


// locals
#include "TrackballControls.h"		// a trackball controller


using namespace std;



#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 1200




namespace cs557
{

	/*!
	This function initializes the GLFW window
	@return - a pointer storing a handle to the window
	*/
	GLFWwindow* initWindow(string window_name = "OpenGL Window for 557");


	/*
	Return a reference of the trackball.
	*/
	TrackballControls& GetTrackball(void);


}