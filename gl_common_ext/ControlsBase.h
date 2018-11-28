#ifndef CONTROLSBASE
#define CONTROLSBASE

// stl
#include <iostream>
#include <algorithm>
#include <limits>

// glfw
#include <GLFW/glfw3.h>


// glm
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_inverse.hpp>

using namespace std;

/*
Abnstract base class for all control classes. 
*/
class ControlsBase
{
public:

     /*
    Return a camera view matrix including the current user interaction
    */
    virtual glm::mat4 getViewMatrix(void) = 0;



    /*
    Deprecated: Return  a rotation matrix within the camera coordinate.
    Do not use this. Use getViewMatrix() instead
    LEGACY FUNCTION - TO BE BACKWARD COMPATIBLE UNTIL I CHANGE THIS
     */
	virtual	glm::mat4 getRotationMatrix() = 0;

private:

};




#endif