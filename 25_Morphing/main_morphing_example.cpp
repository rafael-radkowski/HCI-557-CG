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

#include "GLObjectObj.h"

using namespace std;


// The handle to the window object
GLFWwindow*         window;

// Define some of the global variables we're using for this sample
GLuint program;

/* A trackball to move and rotate the camera view */
extern Trackball trackball;


//////////////////////////////////////////////////////////////////////
// Morphing
//
// The morphing operation requires three steps
// 1. Load a normal model and the morphed model of the object you like to morph.
// 2. Calculate a difference vector (blend shaphe morphing). This happens in function init_morphing()
// 3. During the main loop - call a function that calculates the current animation time fraction and updates the
//      vertices.

GLObjectObj* loadedModel1 = NULL; // the basic / normal model
GLObjectObj* loadedModel2 = NULL; // the morphed model

GLfloat* vector_diff = 0;
GLfloat* vector_result = 0;

/*!
 @brief returns the time fraction for a given time and animation duration
 @param time - the current animation time, application runtime, etc. in seconds
 @param duration - the duration of the animation in seconds
 @return the time fraction in an interval between 0 and 1.
 */
float getTimeFraction(const float time, const float duration)
{
    // we cast to an int. this results in the number of
    float interval = floor(time/duration);
    
    // return the current interval time
    float current_interval = time - interval*duration;
    
    // return the fraction / position in our current timeline
    float fraction = current_interval / duration;

    return fraction;
}




/*!
This initializes the difference vector for the morphing operation
*/
void init_morphing(void)
{
    // allocate memory for the difference vector.
    vector_diff = (GLfloat*)malloc(sizeof(GLfloat) * 3 * loadedModel1->size());
    
    int size = loadedModel1->size();
    // calculate the difference vector
    for (int i=0; i<size; i++) {
        vector_diff[i*3+0] = loadedModel1->getVertices()[i].x - loadedModel2->getVertices()[i].x;
        vector_diff[i*3+1] = loadedModel1->getVertices()[i].y - loadedModel2->getVertices()[i].y;
        vector_diff[i*3+2] = loadedModel1->getVertices()[i].z - loadedModel2->getVertices()[i].z;
    }
    
}


/*!
This function processes the morphing operation
*/
void my_morphing_callback(float duration)
{
    // get the current system time
    float time = glfwGetTime();

    // calcualte the time fraction
    float fraction = getTimeFraction(time,duration);

    // get the number of vertices.
    int size = loadedModel1->size();
    
    // init the vector the first time we call this function.
    if(vector_result == NULL)
    {
        vector_result = (GLfloat*)malloc(sizeof(GLfloat) * 3 * loadedModel1->size());
    }
    
    for (int i=0; i<size; i++) {
        vector_result[i*3+0] = loadedModel1->getVertices()[i].x + fraction * vector_diff[i*3+0];
        vector_result[i*3+1] = loadedModel1->getVertices()[i].y + fraction * vector_diff[i*3+1];
        vector_result[i*3+2] = loadedModel1->getVertices()[i].z + fraction * vector_diff[i*3+2];
    }
    
    
    //---------------------------
    // Draw the output
    loadedModel1->updateVertices(vector_result);
    

}



int main(int argc, const char * argv[])
{
    
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Init glfw, create a window, and init glew
    
    // Init the GLFW Window
    window = initWindow();
    
    
    // Init the glew api
    initGlew();
    SetCameraManipulator(CameraTypes::CAMERA_MANIPULATOR);

    

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Create some models
    
    // coordinate system
    CoordSystem* cs = new CoordSystem(40.0);
    
    
    // create an apperance object.
    GLAppearance* apperance_0 = new GLAppearance("../../data/shaders/multi_vertex_lights.vs", "../../data/shaders/multi_vertex_lights.fs");
    
    GLDirectLightSource  light_source;
    light_source._lightPos = glm::vec4(20.0,20.0,0.0, 0.0);
    light_source._ambient_intensity = 0.2;
    light_source._specular_intensity = 1.5;
    light_source._diffuse_intensity = 2.0;
    light_source._attenuation_coeff = 0.0;
    
    // add the light to this apperance object
    apperance_0->addLightSource(light_source);
    
    // Create a material object
    GLMaterial material_0;
    material_0._diffuse_material = glm::vec3(0.8, 0.8, 0.8);
    material_0._ambient_material = glm::vec3(0.8, 0.8, 0.8);
    material_0._specular_material = glm::vec3(0.1, 0.1, 0.1);
    material_0._shininess = 12.0;
    material_0._transparency = 1.0;
    
    // Add the material to the apperance object
    apperance_0->setMaterial(material_0);
    apperance_0->finalize();
    

    // If you want to change appearance parameters after you init the object, call the update function
    apperance_0->updateLightSources();

    
    loadedModel1 = new GLObjectObj("../../data/dphn1.obj");
    loadedModel1->setApperance(*apperance_0);
    loadedModel1->init();
    

    glm::mat4 tranform = glm::scale(glm::vec3(10.0, 10.0f, 10.0f));
    loadedModel1->setMatrix(tranform);
    
    
    loadedModel2 = new GLObjectObj("../../data/dphn2.obj");
    loadedModel2->setApperance(*apperance_0);
    loadedModel2->init();
    loadedModel2->setMatrix(tranform);
    
    
    // init the difference vector for the morphing operation.
    init_morphing();
    
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Main render loop
    
    // Set up our green background color
    static const GLfloat clear_color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    static const GLfloat clear_depth[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    // This sets the camera to a new location
    // the first parameter is the eye position, the second the center location, and the third the up vector. 
    SetViewAsLookAt(glm::vec3(12.0f, 12.0f, 65.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    
    // Enable depth test
    // ignore this line, it allows us to keep the distance value after we proejct each object to a 2d canvas.
    glEnable(GL_DEPTH_TEST);
    
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Blending
    
    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    

    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Main render loop
    
    // This is our render loop. As long as our window remains open (ESC is not pressed), we'll continue to render things.
    while(!glfwWindowShouldClose(window))
    {
        
        // Clear the entire buffer with our green color (sets the background to be green).
        glClearBufferfv(GL_COLOR , 0, clear_color);
        glClearBufferfv(GL_DEPTH , 0, clear_depth);
        
        //---
        // this morphes the model.
        my_morphing_callback(3.0);
    
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //// This renders the objects
        
        // Set the trackball locatiom
        SetTrackballLocation(GetCurrentCameraMatrix(), GetCurrentCameraTranslation());
        
        // draw the objects
        cs->draw();
        
        loadedModel1->draw();
       
        
       
        //// This renders the objects
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        
        // Swap the buffers so that what we drew will appear on the screen.
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
    
    delete cs;


}

