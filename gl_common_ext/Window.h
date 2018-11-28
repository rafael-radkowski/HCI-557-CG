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
#include "CameraControls.h"

using namespace std;



#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 1024




namespace cs557
{

	/*!
	This function initializes the GLFW window
	@return - a pointer storing a handle to the window
	*/
	GLFWwindow* initWindow(string window_name = "OpenGL Window for 557");


	/*
	Deprecated: Do not use this. Use GetCamera() instead
	Return a reference of the trackball.
    LEGACY FUNCTION - TO BE BACKWARD COMPATIBLE UNTIL I CHANGE THIS
	*/
	ControlsBase& GetTrackball(void);

	/*
	Returns the camera controller. 
	*/
	ControlsBase& GetCamera(void);
	

	/*
	Set the initial view matrix for the camera controler. 
	@param vm - 4x4 view matrix. 
	*/
	void InitControlsViewMatrix(glm::mat4 vm);

}