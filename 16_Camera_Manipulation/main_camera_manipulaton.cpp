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
#include "GLObjects3D.h"

#include "GLAppearance.h"

using namespace std;


// The handle to the window object
GLFWwindow*         window;

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
    
    
    // create an apperance object.
    GLAppearance* apperance_0 = new GLAppearance("../../data/shaders/multi_vertex_lights.vs", "../../data/shaders/multi_vertex_lights.fs");
    
    GLDirectLightSource  light_source;
    light_source._lightPos = glm::vec4(20.0,20.0,0.0, 0.0);
    light_source._ambient_intensity = 0.2;
    light_source._specular_intensity = 5.5;
    light_source._diffuse_intensity = 2.0;
    light_source._attenuation_coeff = 0.0;
    
    // add the light to this apperance object
    apperance_0->addLightSource(light_source);
    
    // Create a material object
    GLMaterial material_0;
    material_0._diffuse_material = glm::vec3(1.0, 0.0, 0.2);
    material_0._ambient_material = glm::vec3(1.0, 0.0, 0.2);
    material_0._specular_material = glm::vec3(1.0, 1.0, 1.0);
    material_0._shininess = 12.0;
    material_0._transparency = 0.4;
    
    // Add the material to the apperance object
    apperance_0->setMaterial(material_0);
    apperance_0->finalize();
    
    // create the sphere geometry
    GLSphere3D* sphere_0 = new GLSphere3D(0.0, 0.0, 0.0, 10.0, 90, 50);
    sphere_0->setApperance(*apperance_0);
    sphere_0->init();
    
    // If you want to change appearance parameters after you init the object, call the update function
    apperance_0->updateLightSources();

    
    
    
    // create a second apperance object.
    GLAppearance* apperance_1 = new GLAppearance("../../data/shaders/multi_vertex_lights.vs", "../../data/shaders/multi_vertex_lights.fs");
    
    // add the light to this apperance object
    apperance_1->addLightSource(light_source);
    
    // Create a material object
    GLMaterial material_1;
    material_1._diffuse_material = glm::vec3(1.0, 1.0, 0.0);
    material_1._ambient_material = glm::vec3(1.0, 1.0, 0.0);
    material_1._specular_material = glm::vec3(1.0, 1.0, 1.0);
    material_1._shininess = 8.0;
    material_1._transparency = 1.0;
    
    // Add the material to the apperance object
    apperance_1->setMaterial(material_1);
    apperance_1->finalize();
    
    GLBox3D* box_1 = new GLBox3D( 20.0, 20.0, 20.0);
    box_1->setApperance(*apperance_1);
    box_1->init();
    
    glm::mat4 tranform = glm::translate(glm::mat4(1.0f), glm::vec3(-25.0f, 0.0f, 0.0f));
    box_1->setMatrix(tranform);

    

    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Main render loop
    
    // Set up our green background color
    static const GLfloat clear_color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    static const GLfloat clear_depth[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    // This sets the camera to a new location
    // the first parameter is the eye position, the second the center location, and the third the up vector. 
    SetViewAsLookAt(glm::vec3(0.0f, 0.0f, 85.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    SetCameraManipulator(CAMERA_MANIPULATOR);
    
    
    
    // Enable depth test
    // ignore this line, it allows us to keep the distance value after we proejct each object to a 2d canvas.
    glEnable(GL_DEPTH_TEST);
    
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Main render loop
    
    // This is our render loop. As long as our window remains open (ESC is not pressed), we'll continue to render things.
    while(!glfwWindowShouldClose(window))
    {
        
        // Clear the entire buffer with our green color (sets the background to be green).
        glClearBufferfv(GL_COLOR , 0, clear_color);
        glClearBufferfv(GL_DEPTH , 0, clear_depth);
        
        
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //// This renders the objects
        
        // Set the trackball locatiom
        SetTrackballLocation(GetCurrentCameraMatrix(), GetCurrentCameraTranslation());
        
        
        // draw the objects
        cs->draw();
        
        box_1->draw();
        sphere_0->draw();
        
       
        //// This renders the objects
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        
        // Swap the buffers so that what we drew will appear on the screen.
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
    
    delete cs;


}

