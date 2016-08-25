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
#include "camera.h"
#include "GLObjectObj.h"

using namespace std;



#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600






/*!
 This function initializes the GLFW window
 */
// The handle to the window object
GLFWwindow* initWindow(void);


/*!
 This function initializes glew
 */
bool initGlew(void);


/*!
 Return the glsl major / minor version.
 */
int GLSLMajor(void);
int GLSLMinor(void);


////////////////////////////////////////////////////////////////////////
// Camera Operations


typedef enum cameraTypes{
    TRACKBALL,
    CAMERA_MANIPULATOR
} CameraTypes;


/**
 Returns the camera matrix of the current active camera object
 */
glm::mat4 GetCurrentCameraMatrix(void);


/**
 Returns the translation matrix of the current camera
*/
glm::vec3 GetCurrentCameraTranslation(void);



/**
 @brief Change the camera manipulator type
 @param m - the camera type.
*/
void SetCameraManipulator(CameraTypes m);


/*!
Set the distance of the camera to your center position
*/
void SetCameraDistance(float distance);



/**
@brief: returns the x and y position of the mouse.
*/
int GetMouseX(void);
int GetMouseY(void);





