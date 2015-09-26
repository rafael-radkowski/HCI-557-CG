//
//  main_spotlight.cpp
//  HCI 557 Spotlight example
//
//  Created by Rafael Radkowski on 5/28/15.
//  Copyright (c) 2015 -. All rights reserved.
//

// stl include
#include <iostream>
#include <string>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


// glfw includes
#include <GLFW/glfw3.h>


// include local files
#include "controls.h"
#include "HCI557Common.h"
#include "CoordSystem.h"
#include "GLSphereDirect.h"




using namespace std;


// The handle to the window object
GLFWwindow*         window;

// Define some of the global variables we're using for this sample
GLuint program;

/* A trackball to move and rotate the camera view */
extern Trackball trackball;



int main(int argc, const char * argv[])
{
    
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Init glfw, create a window, and init glew
    
    // Init the GLFW Window
    window = initWindow();
    
    
    // Init the glew api
    initGlew();
    
    

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Create some models
    
    // coordinate system
    CoordSystem* cs = new CoordSystem(40.0);
    
    
    GLSphereDirect* sphere = new GLSphereDirect(0.0, 0.0, 0.0, 10.0, 90, 50);

    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Main render loop
    
    // Set up our green background color
    static const GLfloat clear_color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    static const GLfloat clear_depth[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    // This sets the camera to a new location
    // the first parameter is the eye position, the second the center location, and the third the up vector. 
    SetViewAsLookAt(glm::vec3(12.0f, 12.0f, 15.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    
    // Enable depth test
    // ignore this line, it allows us to keep the distance value after we proejct each object to a 2d canvas.
    glEnable(GL_DEPTH_TEST);
    
    //sphere->enableNormalVectorRenderer();
    
    // This is our render loop. As long as our window remains open (ESC is not pressed), we'll continue to render things.
    while(!glfwWindowShouldClose(window))
    {
        
        // Clear the entire buffer with our green color (sets the background to be green).
        glClearBufferfv(GL_COLOR , 0, clear_color);
        glClearBufferfv(GL_DEPTH , 0, clear_depth);
        
    
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //// This renders the objects
        
        // Set the trackball locatiom
        SetTrackballLocation(trackball.getRotationMatrix());
        
        // draw the objects
        cs->draw();
        
        sphere->draw();
        
        //// This renders the objects
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        
        // Swap the buffers so that what we drew will appear on the screen.
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
    
    delete cs;


}

