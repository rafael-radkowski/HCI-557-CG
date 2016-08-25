/**
 Camera manipulation example.
 This code is a simple example for camera manipulation.

 This code is an example of ME/CprE/ComS 557
 It keeps a set of example objects for the code
 examples of the course.

 Rafael Radkowski
 Iowa State University
 rafael@iastate.edu
 Fall 2015
**/

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
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_inverse.hpp>



// glfw includes
#include <GLFW/glfw3.h>


using namespace std;

class CameraBase
{
public:

    CameraBase(){};
    
    /**
     
     */
    virtual void camera_MouseCursor_Callack( GLFWwindow *window, double x, double y) = 0;
    
    
    /**
     
     */
    virtual void camera_MouseButton_Callback(GLFWwindow * window, int button, int action, int mods) = 0;
    
    /*
     * Create a rotation matrix within the camera coordinate,
     */
    virtual glm::mat4 getRotationMatrix(void) = 0;
 
    /*!
     Gets the translation matrix to adjust the distance between camera and object
     */
    virtual glm::vec3 getTranslationVec(void) = 0;
};


class CameraManipulator : public CameraBase
{
public:
    
    CameraManipulator(int window_width, int window_height, float rolling_speed );
    ~CameraManipulator();
    
    /**
 
     */
    void init_camera_param(void);

    /**

     */
    virtual void camera_MouseCursor_Callack( GLFWwindow *window, double x, double y);


    /**
     
     */
    virtual void camera_MouseButton_Callback(GLFWwindow * window, int button, int action, int mods) ;

    
    /*!
     Convert the mouse cursor position from screen coordinates to window coordinates
     */
    glm::vec3 toWindowCoord( double x, double y );
    

    /*!
     Returns the current rotation matrix
     */
    virtual glm::mat4 getRotationMatrix(void);
    
    
    /*!
     Gets the translation matrix to adjust the distance between camera and object
     */
    virtual glm::vec3 getTranslationVec(void);
    
    
    /*!
    Set the distance of your camera.
    */
    inline void setCameraDistance(float distance){if(distance>1.0)_camera_distance=distance;}

    
protected:
    
    int             _window_width;
    int             _window_height;
    
    
    int             _leftMouseButtonEvent;
    int             _rightMouseButtonEvent;
    
    // the start position of the mouse when the left mouse button is pressed
    glm::vec3       _startPos;
    glm::vec3       _currentPos;
    

    // values to calculate the distance for the camera
    glm::vec3       _lastframePosDistance;
    glm::vec3       _currentPosDistance;
    
    // The angle between _startPos and _currentPos
    float          _currentAngle;
    
    // The current axis for rotation as cross product
    glm::vec3       _cameraRotationAxis;
    
    // the rolling speed for this trackball
    float          _rolling_speed;
    
    
    float          _traveling_speed;
    float          _camera_distance;
};



