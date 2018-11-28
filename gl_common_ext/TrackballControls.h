#pragma once
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

using namespace glm;
using namespace std;



namespace cs557
{

	/*
	 Implementation of a trakball
	 */
	class TrackballControls : public ControlsBase
	{

	public:

		TrackballControls(int window_width, int window_height, GLfloat roll_speed = 0.5f, bool x_axis = true, bool y_axis = true);


		glm::vec3 toScreenCoord(double x, double y);


		void mouseButtonCallback(GLFWwindow * window, int button, int action, int mods);


		void cursorCallback(GLFWwindow *window, double x, double y);


		/*
		Deprecated: Do not use this. Use GetViewMatrix() instead
		Return a reference of the trackball.
    	LEGACY FUNCTION - TO BE BACKWARD COMPATIBLE UNTIL I CHANGE THIS
		*/
		glm::mat4 getRotationMatrix();



		/*
		Return a camera view matrix including the current user interaction
		*/
		glm::mat4 getViewMatrix(void);



	private:

		int     _windowWidth;
		int     _windowHeight;
		int     _mouseEvent;

		GLfloat _rollingSpeed;
		GLfloat _current_angle;

		glm::vec3   _prevPos;
		glm::vec3   _currPos;
		glm::vec3   _camAxis;

		glm::mat4   _curOrient;

		bool        _xAxis;
		bool        _yAxis;

		 // the view matrix. 
        glm::mat4       _vm;

	};


}