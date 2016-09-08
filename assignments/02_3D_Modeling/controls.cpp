// Include GLFW
#include <GLFW/glfw3.h>


// stl
#include <iostream>


// glm
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_inverse.hpp>



// local
#include "controls.h"



using namespace glm;


// The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.
extern GLFWwindow* window;


/**
 * Constructor.
 * @param roll_speed the speed of rotation
 */
Trackball::Trackball( int window_width, int window_height, GLfloat roll_speed, bool x_axis, bool y_axis  ):
_windowWidth(window_width), _windowHeight(window_height), _rollingSpeed(roll_speed),_xAxis(x_axis), _yAxis(y_axis)
{
    
    _mouseEvent = 0;
    _current_angle    = 0.0f;
    _camAxis    = glm::vec3(0.0f, 1.0f, 0.0f);
    
    _last_rotation = glm::mat4(1.0f);

}

/**
 * Convert the mouse cursor coordinate on the window (i.e. from (0,0) to (windowWidth, windowHeight))
 * into normalized screen coordinate (i.e. (-1, -1) to (1, 1)
 */
glm::vec3 Trackball::toScreenCoord( double x, double y ) {
    glm::vec3 coord(0.0f);
    
    if( _xAxis )
        coord.x =  (2 * x - _windowWidth ) / _windowWidth;
    
    if( _yAxis )
        coord.y = -(2 * y - _windowHeight) / _windowHeight;
    
    /* Clamp it to border of the windows */
    coord.x = glm::clamp( coord.x, -1.0f, 1.0f );
    coord.y = glm::clamp( coord.y, -1.0f, 1.0f );
    
    float length_squared = coord.x * coord.x + coord.y * coord.y;
    if( length_squared <= 1.0 )
        coord.z = sqrt( 1.0 - length_squared );
    else
        coord = glm::normalize( coord );
    
    return coord;
}

/**
 * Check whether we should start the mouse event
 * Event 0: when no tracking occured
 * Event 1: at the start of tracking, recording the first cursor pos
 * Event 2: tracking of subsequent cursor movement
 */
void Trackball::mouseButtonCallback( GLFWwindow * window, int button, int action, int mods ){
    _mouseEvent = ( action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT );
}

void Trackball::cursorCallback( GLFWwindow *window, double x, double y ){
    if( _mouseEvent == 0 )
        return;
    else if( _mouseEvent == 1 ) {
        /* Start of trackball, remember the first position */
        _prevPos     = toScreenCoord( x, y );
        _mouseEvent  = 2;
        return;
    }
    
    /* Tracking the subsequent */
    _currPos  = toScreenCoord( x, y );
    
    /* Calculate the angle in radians, and clamp it between 0 and 90 degrees */
    _current_angle    = acos( std::min(1.0f, glm::dot(_prevPos, _currPos) ));
    
    /* Cross product to get the rotation axis, but it's still in camera coordinate */
    _camAxis  = glm::cross( _prevPos, _currPos );

}


glm::mat4 Trackball::getRotationMatrix() {
    
	glm::mat4 rot = glm::rotate( glm::degrees(_current_angle) * _rollingSpeed, _camAxis );
	glm::mat4 current_rot = _last_rotation * rot;
	_last_rotation = rot;
    return current_rot;
}

/**
 * Create rotation matrix within the world coordinate,
 * multiply this matrix with model matrix to rotate the object
 */
glm::mat4 Trackball::createModelRotationMatrix( glm::mat4& view_matrix ){
    
    glm::vec3 axis = glm::inverse(glm::mat3(view_matrix)) * _camAxis;
    
    return glm::rotate( glm::degrees(_current_angle) * _rollingSpeed, axis );
}





