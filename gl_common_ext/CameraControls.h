#ifndef TRACKBALLCONTROLS
#define TRACKBALLCONTROLS

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


#include "ControlsBase.h"

using namespace std;


namespace cs557
{

    class CameraControls : public ControlsBase
    {
    public:
        /*
        Constructor
        @param window_width: width of the window in pixels.
        @param window_height: height of the window in pixels. 
        */
        CameraControls(int window_width, int window_height);

        /*
        Set an initial view matrix to define the start point. 
        @parma vm - a 4x4 view matrix. 
        */
        void initView(glm::mat4 vm);


        /*
		Return a camera view matrix including the current user interaction
		*/
		glm::mat4 getViewMatrix(void);


        /*
		Deprecated: Do not use this. Use GetViewMatrix() instead
		Return a reference of the trackball.
    	LEGACY FUNCTION - TO BE BACKWARD COMPATIBLE UNTIL I CHANGE THIS
		*/
        glm::mat4 getRotationMatrix(void); 

        /*
        Mouse cursor callback. 
        */
        void cursorCallback( GLFWwindow *window, double x, double y );


        /*
        Mouse button callback function
        */
        void mouseButtonCallback( GLFWwindow * window, int button, int action, int mods );


        /*
        Forward keyboard interaction to this function so that the controller can react to keyboard buttons
        */
        void forwardKeyboardInfo( int key, int scancode, int action, int mods);


    private:

        /*
         Convert the mouse cursor coordinate on the window (i.e. from (0,0) to (windowWidth, windowHeight))
         into normalized screen coordinate (i.e. (-1, -1) to (1, 1)
         */
        glm::vec3 toScreenCoord( double x, double y );
        

        //////////
        /// Memebers

        // the view matrix. 
        glm::mat4       _vm;


        int             _windowWidth;
		int             _windowHeight;

        glm::vec3       _position;
        float           _horizontalAngle;
        float           _verticalAngle;
        float           _locomotion_speed; 
        float           _rolling_speed;

        bool            _xAxis;
        bool            _yAxis;

        /*
        0 - no event
        1 - left mouse button pressed. 
        */
        int             _mouse_event;

        // screen coordinates
        glm::vec3       _previous_sc; 
        glm::vec3       _current_sc; 

        glm::vec3       _camAxis;
        float           _current_angle;
        
    };

}//namespace cs557


#endif