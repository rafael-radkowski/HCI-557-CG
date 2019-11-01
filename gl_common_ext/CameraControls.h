/*
@file: CameraControls.h/.cpp

This file provides a trackball mouse controller for camera/mouse navigation.

This file is part of CS/CPRE/ME 557 Computer Graphics at Iowa State University

Rafael Radkowski
Iowa State University
rafael@iastate.edu
+1 (515) 294-7044
MIT License

-------------------------------------------------------------------------------
Last edited:

Oct 19, 2019, RR
- Added a angle-check to the cursorCallback. The viewmatrix becomes undefined if
  the angle is < 0.001. All objects would disappear. To prevent that, I now check whether the
  angle is < 0.001.
*/


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

       
        float           _horizontalAngle;
        float           _verticalAngle;
        float           _locomotion_speed; 
        float           _rolling_speed;

        bool            _xAxis;
        bool            _yAxis;

		glm::vec3       _center;
		glm::vec3       _eye;

		glm::vec2		_prev_screen_xy;

        /*
        0 - no event
        1 - left mouse button pressed. 
        */
        int             _mouse_event;
		int             _mouse_move_event;
		int				_mouse_middle_event;

        // screen coordinates
        glm::vec3       _previous_sc; 
        glm::vec3       _current_sc; 

        glm::vec3       _camAxis;
        float           _current_angle;
        
    };

}//namespace cs557


#endif