#include "CameraControls.h"



using namespace cs557;


 /*
Constructor
@param window_width: width of the window in pixels.
@param window_height: height of the window in pixels. 
*/
CameraControls::CameraControls(int window_width, int window_height):
    _windowWidth(window_width), _windowHeight(window_height)
{

    _xAxis = true;
    _yAxis = true;

    _position = glm::vec3(0.0,0.0,-6.0);
    _horizontalAngle = 3.14f;
    _verticalAngle = 0.0f;
    _locomotion_speed = 0.2f; 
    _rolling_speed = 0.5f;
    _mouse_event = 0;
    _current_angle = 0;

    // an initial matrix to start with. 
    _vm = glm::lookAt(_position, glm::vec3(0.0,0.0,0.0), glm::vec3(0.0,1.0,0.0) );

}

/*
Set an initial view matrix to define the start point. 
@parma vm - a 4x4 view matrix. 
*/
void CameraControls::initView(glm::mat4 vm)
{
    _vm = vm;
    _position = _position = glm::vec3(-_vm[3][0],-_vm[3][1],-_vm[3][2]);
}


/*
Return a camera view matrix including the current user interaction
*/
glm::mat4 CameraControls::getViewMatrix(void)
{
    return _vm;
}

glm::mat4 CameraControls::getRotationMatrix() 
{
    return _vm;  
}


/*
Convert the mouse cursor coordinate on the window (i.e. from (0,0) to (windowWidth, windowHeight))
into normalized screen coordinate (i.e. (-1, -1) to (1, 1)
*/
glm::vec3 CameraControls::toScreenCoord( double x, double y ) 
{
    glm::vec3 coord(0.0f);
    
    if( _xAxis )
        coord.x =  -(2 * x - _windowWidth ) / _windowWidth;
    
    if( _yAxis )
        coord.y = -(2 * y - _windowHeight) / _windowHeight;
    
    /* Clamp it to border of the windows */
    coord.x = glm::clamp( coord.x, -1.0f, 1.0f );
    coord.z = glm::clamp( coord.y, -1.0f, 1.0f );
    
    
    float length_squared = coord.x * coord.x + coord.z * coord.z;
    if( length_squared <= 1.0 )
        coord.y = sqrt( 1.0 - length_squared );
    else
        coord = glm::normalize( coord );
    
    return coord;
}


 /*
Mouse cursor callback. 
*/
void CameraControls::cursorCallback( GLFWwindow *window, double x, double y )
{

    if(_mouse_event == 1)
    {
        _previous_sc = toScreenCoord( x, y ); 
        _mouse_event = 2;
    }
    else if(_mouse_event == 2)
    {
        _current_sc= toScreenCoord( x, y ); 
        /* Calculate the angle in radians, and clamp it between 0 and 90 degrees */
        _current_angle    = acos( std::min(1.0f, glm::dot(_previous_sc, _current_sc) ));

         /* Cross product to get the rotation axis, but it's still in camera coordinate */
        _camAxis  = glm::cross( _previous_sc, _current_sc );
		
		// Check if the angle is larger than 0.001. The matrix becomes undefined otherwise. 
		if(std::abs(_current_angle) > 0.001)
			_vm = glm::rotate(_vm, glm::degrees(_current_angle) * _rolling_speed, _camAxis );

        _previous_sc = _current_sc;
    }
    
}

/*
Mouse button callback function
*/
void CameraControls::mouseButtonCallback( GLFWwindow * window, int button, int action, int mods )
{
    //  mouse event
     _mouse_event = ( action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT );

}


  /*
Forward keyboard interaction to this function so that the controller can react to keyboard buttons
*/
void CameraControls::forwardKeyboardInfo( int key, int scancode, int action, int mods)
{


}