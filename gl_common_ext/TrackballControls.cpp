// local
#include "TrackballControls.h"




using namespace cs557;



/**
 * Constructor.
 * @param roll_speed the speed of rotation
 */
TrackballControls::TrackballControls( int window_width, int window_height, GLfloat roll_speed, bool x_axis, bool y_axis  ):
    _windowWidth(window_width), _windowHeight(window_height), _rollingSpeed(roll_speed),_xAxis(x_axis), _yAxis(y_axis)
{
    
    _mouseEvent = 0;
    _current_angle    = 0.0f;
    _camAxis    = glm::vec3(0.0f, 1.0f, 0.0f);
    
    _vm = { 1,0,0,0,
            0,1,0,0,
            0,0,1,0,
            0,0,0,1};

    _curOrient = glm::mat4();
}

/**
 * Convert the mouse cursor coordinate on the window (i.e. from (0,0) to (windowWidth, windowHeight))
 * into normalized screen coordinate (i.e. (-1, -1) to (1, 1)
 */
glm::vec3 TrackballControls::toScreenCoord( double x, double y ) {
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
void TrackballControls::mouseButtonCallback( GLFWwindow * window, int button, int action, int mods ){
    _mouseEvent = ( action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT );


}



void TrackballControls::cursorCallback( GLFWwindow *window, double x, double y ){
    if( _mouseEvent == 0 ){
        return;
    }
    else if( _mouseEvent == 1 ) {
        /* Start of trackball, remember the first position */
        _prevPos     = toScreenCoord( x, y );
        _mouseEvent  = 2;
        return;
    }
    else if(_mouseEvent == 2)
    {
    
        /* Tracking the subsequent */
        _currPos  = toScreenCoord( x, y );
        
        /* Calculate the angle in radians, and clamp it between 0 and 90 degrees */
        _current_angle    = acos( std::min(1.0f, glm::dot(_prevPos, _currPos) ));
        
        /* Cross product to get the rotation axis, but it's still in camera coordinate */
        _camAxis  = glm::cross( _prevPos, _currPos );


        _vm = glm::rotate( _vm, glm::degrees(_current_angle) * _rollingSpeed, _camAxis );

        _prevPos = _currPos;
    
    }

}




glm::mat4 TrackballControls::getRotationMatrix() {
    return _vm;  
}

/*
Return a camera view matrix including the current user interaction
*/
glm::mat4 TrackballControls::getViewMatrix() {
    return _vm;  
}





