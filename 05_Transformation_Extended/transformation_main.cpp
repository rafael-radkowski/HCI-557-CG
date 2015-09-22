//
//  main.cpp
//  OpenGL4Test
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
#include "GLColoredBox.h"


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
    
    

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Create some models
    
    // coordinate system
    CoordSystem* cs = new CoordSystem(4.0);
    
    // This example demonstrate how to invert the model matrix of an box that is not at its location.
    // Rotate models only when they are at the orign. Otherwise, you rotate them with a lever.
    
    // three identical boxes
    // on its original location.
    GLColoredBox* box_rotated_at_origin = new GLColoredBox(0.5,1.0,0.5);
    
    // This box will be rotated first and then translated
    GLColoredBox* box_rotated_at_curr_location = new GLColoredBox(0.5,1.0,0.5);
    
    GLColoredBox* box = new GLColoredBox(0.5,1.0,0.5);
    
    
    // These matrices define the shifted location of all our objects. We suppose, they
    // are not at their origin locaiton
    glm::mat4 translation_first = glm::translate(glm::vec3(2.0f, 0.0f, 0.0f));
    glm::mat4 rotation_first = glm::rotate( 0.57f, glm::vec3(0.0f, 0.0f, 1.0f));
    
    glm::mat4 matrix_first = translation_first * rotation_first;

    // Here we transform the objects for the first time.
    box_rotated_at_origin->setModelMatrix(matrix_first);
    box_rotated_at_curr_location->setModelMatrix(matrix_first);
    box->setModelMatrix(matrix_first);
    
    // Let's assume, we want to further rotate and translate the models.
    glm::mat4 translation_second = glm::translate(glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 rotation_second = glm::rotate( 1.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    
    // This returns the current model matrix of each object
    glm::mat4& current_location_0 = box_rotated_at_origin->getModelMatrix();
    glm::mat4& current_location_1 = box_rotated_at_curr_location->getModelMatrix();
    
    // here we take the current model matrix, and rotate first, next we translate.
    glm::mat4 matrix_second_1 =  translation_second *  rotation_second * current_location_1;
    
    // Here we rotate and add all objects in the order, rotation first, then translation.
    glm::mat4 matrix_second_2 =  translation_second * translation_first * rotation_second * rotation_first;
    
    // set the object to its new location
    box_rotated_at_curr_location->setModelMatrix(matrix_second_1);
    box_rotated_at_origin->setModelMatrix(matrix_second_2);
    
    // KEEP IN MIND: THERE IS NO RIGHT AND WRONG WAY. HOWEVER, THE RESULTS ARE ALWAYS DIFFERENT.
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Main render loop
    
    // Set up our green background color
    static const GLfloat clear_color[] = { 0.6f, 0.7f, 1.0f, 1.0f };
    static const GLfloat clear_depth[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    // This sets the camera to a new location
    // the first parameter is the eye position, the second the center location, and the third the up vector. 
    SetViewAsLookAt(glm::vec3(1.0f, 1.0f, 2.5f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    
    // Enable depth test
    // ignore this line, it allows us to keep the distance value after we proejct each object to a 2d canvas.
    glEnable(GL_DEPTH_TEST);
    
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
        
        box_rotated_at_origin->draw();
        
        box_rotated_at_curr_location->draw();
        
        box->draw();
        
        //// This renders the objects
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        
        // Swap the buffers so that what we drew will appear on the screen.
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
    
    delete cs;
    delete box_rotated_at_origin;
    delete box_rotated_at_curr_location;

}

