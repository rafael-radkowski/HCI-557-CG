#include "Window.h"


//#define _WITH_CAMERA

namespace cs557
{


	// A trackball to move and rotate the camera view 
	// Note that WINDOW_WIDTH and  WINDOW_HEIGHT is defined in Window.h
#ifdef _WITH_CAMERA
	static cs557::CameraControls camera(WINDOW_WIDTH, WINDOW_HEIGHT);
#else
	static cs557::TrackballControls trackball(WINDOW_WIDTH, WINDOW_HEIGHT, 0.4f, true, true);
#endif



	/* In GLFW mouse callback */
	void mouseButtonCallback(GLFWwindow * window, int button, int action, int mods) {
	
#ifdef _WITH_CAMERA
		camera.mouseButtonCallback(window, button, action, mods);
#else
		trackball.mouseButtonCallback(window, button, action, mods);
#endif
	}



	/* In GLFW curser callback */
	void cursorCallback(GLFWwindow *window, double x, double y) {
	
#ifdef _WITH_CAMERA
		camera.cursorCallback(window, x, y);
#else
		trackball.cursorCallback(window, x, y);
#endif
	}



	// This is the callback we'll be registering with GLFW for errors.
	// It'll just print out the error to the STDERR stream.
	void error_callback(int error, const char* description)
	{
		fputs(description, stderr);
	}



	// This is the callback we'll be registering with GLFW for keyboard handling.
	// The only thing we're doing here is setting up the window to close when we press ESC
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

	}





	/*
	This function initializes the GLFW window
	*/
	GLFWwindow* initWindow(string window_name)
	{
		// The handle to the window object
		GLFWwindow*         window = NULL;

		// Initialize GLFW, and if it fails to initialize for any reason, print it out to STDERR.
		if (!glfwInit()) {
			fprintf(stderr, "Failed initialize GLFW.");
			exit(EXIT_FAILURE);
		}


		// Set the error callback, as mentioned above.
		glfwSetErrorCallback(error_callback);


		// Set up OpenGL options.
		// Use GLFW verion 3.3,
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


		// Create a window to put our stuff in.
		window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, window_name.c_str() , NULL, NULL);

		// If the window fails to be created, print out the error, clean up GLFW and exit the program.
		if (window == NULL) {
			cout << "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible." << endl;
			glfwTerminate();
			system("pause");
			exit(-1);
		}


		// Use the window as the current context (everything that's drawn will be place in this window).
		glfwMakeContextCurrent(window);


		// Set the keyboard callback so that when we press ESC, it knows what to do.
		glfwSetKeyCallback(window, key_callback);


		// set the cursor callback
		glfwSetCursorPosCallback(window, cursorCallback);
		glfwSetMouseButtonCallback(window, mouseButtonCallback);


		return window;
	}




	/*
	Return a reference of the trackball.
	*/
	ControlsBase& GetTrackball(void)
	{
#ifndef _WITH_CAMERA
		return trackball;	
#else
		return camera;
#endif
	}


	ControlsBase& GetCamera(void)
	{
#ifdef _WITH_CAMERA
		return camera;
#else
		return trackball;	
#endif
	}


	/*
	Set the initial view matrix for the camera controler. 
	@param vm - 4x4 view matrix. 
	*/
	void InitControlsViewMatrix(glm::mat4 vm)
	{
#ifdef _WITH_CAMERA
		camera.initView(vm);
#endif
	}




};//namespace cs557