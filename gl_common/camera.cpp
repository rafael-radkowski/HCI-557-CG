#include "camera.h"
#include <math.h>



CameraManipulator::CameraManipulator(int window_width, int window_height, float rolling_speed ):
    _window_width(window_width), _window_height(window_height), _rolling_speed(rolling_speed)
{
    init_camera_param();
}


CameraManipulator::~CameraManipulator()
{

}



void CameraManipulator::init_camera_param(void)
{
    _leftMouseButtonEvent = 0;
    _rightMouseButtonEvent = 0;
    _startPos = glm::vec3(0.0,0.0,0.0);
    _currentPos = glm::vec3(0.0,0.0,0.0);
    _cameraRotationAxis = glm::vec3(0.0,1.0,0.0);
    _currentAngle = 0.0;
    
    _traveling_speed = 1000.0f;
    _camera_distance = 50.0f;
    
    _lastframePosDistance = glm::vec3(0.0,0.0,0.0);
}


void CameraManipulator::camera_MouseCursor_Callack( GLFWwindow *window, double x, double y)
{

    ///----------------------------
    // Camera distance starts here
    switch (_rightMouseButtonEvent)
    {
        case 0:
            break;
            
        case 1:
            //  remember the first position
            _rightMouseButtonEvent = 2;
            _lastframePosDistance = toWindowCoord( x, y );
            return;
            break;
    }
    
    if(_rightMouseButtonEvent == 2)
    {
        // Tracking the current mouse cursor
        _currentPosDistance  = toWindowCoord( x, y );
        
        float dir =  _lastframePosDistance.y - _currentPosDistance.y; // this order to invert the mouse motion
        
        float length = sqrt( (_currentPosDistance.y - _lastframePosDistance.y)* (_currentPosDistance.y - _lastframePosDistance.y));
        
        _camera_distance = _camera_distance + length * dir * _traveling_speed;
        _camera_distance =  glm::clamp(_camera_distance, 0.0f, 600.0f);

        _lastframePosDistance = _currentPosDistance;
    }
    
    

    ///----------------------------
    // Camera orientation starts here
    
    switch (_leftMouseButtonEvent) {
        case 0:
            return;
            break;
            
        case 1:
            //  remember the first position
            _startPos = toWindowCoord( x, y );
            
            // switch to state 2
            _leftMouseButtonEvent = 2;
            return;
            break;
    }
    
    // state 2 is processed here
    
    // Tracking the current mouse cursor
    _currentPos  = toWindowCoord( x, y );
    
    // Calculate the angle in radians, and clamp it between 0 and 90 degrees
    _currentAngle    = acos( std::min(1.0f, glm::dot(_startPos, _currentPos) ));
    
    // Cross product to get the rotation axis, but it's still in camera coordinate
    _cameraRotationAxis  = glm::cross( _startPos, _currentPos );
    
}



void CameraManipulator::camera_MouseButton_Callback(GLFWwindow * window, int button, int action, int mods)
{
    
    //cout << button << "\t" << action << "\t" << mods << endl;
    
    
    switch (action) {
        case GLFW_PRESS: // 1
            
            if(button == GLFW_MOUSE_BUTTON_1) // Left mouse button = 0
            {
                // switch mouse button event to 1 when the mouse button is pressed
                _leftMouseButtonEvent = 1;

            }
            else if(button == GLFW_MOUSE_BUTTON_2) // Right mouse button = 1
            {
                _rightMouseButtonEvent = 1;
            }
            
            break;
        case GLFW_RELEASE: // 0
            
            if(button == GLFW_MOUSE_BUTTON_1) // Left mouse button = 0
            {
                // switch mouse button event to 0 when the mouse button is release
                _leftMouseButtonEvent = 0;
                
            }
            else if(button == GLFW_MOUSE_BUTTON_2) // Right mouse button = 1
            {
                _rightMouseButtonEvent = 0;
            }
            
            break;
        
    }

}




/**
 * Convert the mouse cursor coordinate on the window (i.e. from (0,0) to (windowWidth, windowHeight))
 * into normalized screen coordinate (i.e. (-1, -1) to (1, 1)
 */
glm::vec3 CameraManipulator::toWindowCoord( double x, double y )
{
    glm::vec3 coord(0.0f);
    
    coord.x =  (2 * x - _window_width ) / _window_width;
    coord.y = -(2 * y - _window_height) / _window_height;
    
    
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


/*!
 Returns the current rotation matrix
 */
//virtual
glm::mat4  CameraManipulator::getRotationMatrix(void)
{
    return glm::rotate( glm::degrees(_currentAngle) * _rolling_speed, _cameraRotationAxis );
}

/*!
 Gets the translation matrix to adjust the distance between camera and object
 */
//virtual
glm::vec3 CameraManipulator::getTranslationVec(void)
{
    return glm::vec3(0.0,0.0,_camera_distance);
}




