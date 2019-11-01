/*
Trackback control for a gl scene

Rafael Radkowski
Iowa State University
rafael@iastate.edu

Jan 15, 2016
MIT License
--------------------------------------------------------------
Last edited:

Oct 26, 2019, RR
- Updated the control features. One can now change the distance to the object using the left mouse button. 
- Bug fix: fixed a bug that prevented the trackball from rotating correctly. 


*/


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

    _center = glm::vec3(0.0,0.0,0.0);
	_eye = glm::vec3(0.0,0.0,-6.0);
    _horizontalAngle = 3.14f;
    _verticalAngle = 0.0f;
    _locomotion_speed = 0.2f; 
    _rolling_speed = 0.5f;
    _mouse_event = 0;
	_mouse_move_event = 0.0;
    _current_angle = 0;
	_mouse_middle_event = 0;

	_prev_screen_xy = glm::vec2(0,0);

    // an initial matrix to start with. 
    _vm = glm::lookAt(_eye, _center, glm::vec3(0.0,1.0,0.0) );

}

/*
Set an initial view matrix to define the start point. 
@parma vm - a 4x4 view matrix. 
*/
void CameraControls::initView(glm::mat4 vm)
{
    _vm = vm;
    _eye = glm::vec3(-_vm[3][0],-_vm[3][1],-_vm[3][2]);
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
    
	float wx = 0.0;
	float wy = 0.0;
    if( _xAxis )
		wx = (2 * x - _windowWidth ) / _windowWidth;
  
    if( _yAxis )
        wy = (2 * y - _windowHeight) / _windowHeight;
    
    /* Clamp it to border of the windows */
	//coord.x = coord.z;
    coord.x = glm::clamp( -wx, -1.0f, 1.0f );
    coord.y = glm::clamp( wy, -1.0f, 1.0f );
   
    
    float length_squared = coord.y * coord.y + coord.x * coord.x;
    if( length_squared <= 1.0 )
       coord.z = sqrt( 1.0 - length_squared );
    else{
        coord = glm::normalize( coord );
	}
    
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
        _current_angle    = acos( std::min(1.0f, glm::dot( _current_sc, _previous_sc) ));

         /* Cross product to get the rotation axis, but it's still in camera coordinate */
        _camAxis  = glm::cross(  _current_sc, _previous_sc );
		
		// Check if the angle is larger than 0.001. The matrix becomes undefined otherwise. 
		if(std::abs(_current_angle) > 0.001){
			_eye = _vm[3];
			_vm[3] = glm::vec4(0.0,0.0,0.0, 1.0);
			_vm = glm::translate(_eye) * (glm::rotate(glm::mat4(1.0), glm::degrees(_current_angle) * _rolling_speed, _camAxis ) * _vm) ;
		}

        _previous_sc = _current_sc;
    }

	if (_mouse_move_event == 1) {
		
		_prev_screen_xy.y = y;
		_prev_screen_xy.x = x;
		_mouse_move_event = 2;

	}else if (_mouse_move_event == 2) {
		
		 float distance = y - _prev_screen_xy.y;
		 float sign = glm::sign( y - _prev_screen_xy.y );

		 glm::mat4 _vm_inv =glm::inverse(_vm);

		 if(std::abs(distance) > 0.001){
			 
			_eye = glm::vec3(_vm[3]) +  glm::vec3( glm::normalize(_vm_inv[1]- _vm[3])) * sign * 0.05f;
			_vm[3] = glm::vec4(0.0,0.0,0.0, 1.0);
			_vm = glm::translate(_eye)  * _vm;
			//_vm[3] = glm::vec4(_eye, 1.0);
		}
		 _prev_screen_xy.y = y;
		 _prev_screen_xy.x = x;
	}


	if (_mouse_middle_event == 1) {
		
		_previous_sc = toScreenCoord( y,x ); 
		_prev_screen_xy.y = y;
		_prev_screen_xy.x = x;
		_mouse_middle_event = 2;

	}
	else if (_mouse_middle_event == 2) {
		
		float signx = glm::sign( x - _prev_screen_xy.x );
		float signy = glm::sign( y - _prev_screen_xy.y );
		_current_sc= toScreenCoord( y,x ); 
		

		glm::mat4 _vm_inv =glm::inverse(_vm);
		
		_eye = glm::vec3(_vm[3]) +  glm::normalize( glm::vec3(_vm_inv[0])) * signx * 0.05f -  glm::normalize( glm::vec3(_vm_inv[1])) * signy * 0.05f;
		_center =  glm::vec3(_vm[3]) + glm::normalize(glm::vec3(_vm[2])) +  glm::normalize( glm::vec3(_vm_inv[0])) * signx * 0.05f -  glm::normalize( glm::vec3(_vm_inv[1])) * signy * 0.05f;

		_vm[3] = glm::vec4(_eye, 1.0);
		_vm[2] = glm::vec4( glm::normalize(_center - _eye), 0.0);

	
		_prev_screen_xy.y = y;
		_prev_screen_xy.x = x;
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
	 _mouse_move_event = ( action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_RIGHT );
	 _mouse_middle_event = ( action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_MIDDLE );
}


  /*
Forward keyboard interaction to this function so that the controller can react to keyboard buttons
*/
void CameraControls::forwardKeyboardInfo( int key, int scancode, int action, int mods)
{


}