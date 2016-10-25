#pragma once
// stl
#include <iostream>
#include <algorithm>

// glfw
#include <GLFW/glfw3.h>


// glm
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_inverse.hpp>




/*
 Implementation of a trakball
 */
class Trackball {
    
public:
    Trackball( int window_width, int window_height, GLfloat roll_speed = 0.5f, bool x_axis = true, bool y_axis = true );
    glm::vec3 toScreenCoord( double x, double y );
    
    void mouseButtonCallback( GLFWwindow * window, int button, int action, int mods );
    void cursorCallback( GLFWwindow *window, double x, double y );
    
    
    /*
     * Create a rotation matrix within the camera coordinate,
     */
    glm::mat4 getRotationMatrix();
    
    
    glm::mat4 createModelRotationMatrix( glm::mat4& view_matrix );
    
private:
    
    int     _windowWidth;
    int     _windowHeight;
    int     _mouseEvent;
    
    GLfloat _rollingSpeed;
    GLfloat _current_angle ;
    
    glm::vec3   _prevPos;
    glm::vec3   _currPos;
    glm::vec3   _camAxis;
    
    bool        _xAxis;
    bool        _yAxis;
    
};
