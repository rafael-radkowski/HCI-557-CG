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


// glfw includes
#include <GLFW/glfw3.h>

// Locals
#include "controls.h"


using namespace std;



#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600


/* A trackball to move and rotate the camera view */
static Trackball trackball( WINDOW_WIDTH, WINDOW_HEIGHT, 0.4f, true, true );





/*!
 This function initializes the GLFW window
 */
// The handle to the window object
GLFWwindow* initWindow(void);


/*!
 This function initializes glew
 */
bool initGlew(void);



