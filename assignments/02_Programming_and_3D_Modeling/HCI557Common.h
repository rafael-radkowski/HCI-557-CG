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




/*!
 This function initializes the GLFW window
 */
// The handle to the window object
GLFWwindow* initWindow(void);


/*!
 This function initializes glew
 */
bool initGlew(void);



