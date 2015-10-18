//
//  main_environment_mapping.cpp
//  HCI 557 Environment mapping.
//  This example demonstrates spherical environment mapping with glsl.
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
#include "Texture.h"
#include "GLObjects3D.h"


using namespace std;


// The handle to the window object
GLFWwindow*         window;



// this is a helper variable to allow us to change the texture blend model
extern int g_change_texture_blend;


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
    GLAppearance* apperance_0 = new GLAppearance("../../data/shaders/spherical_mapping.vs", "../../data/shaders/spherical_mapping.fs");
    
    GLDirectLightSource  light_source;
    light_source._lightPos = glm::vec4(00.0,20.0,20.0, 0.0);
    light_source._ambient_intensity = 0.2;
    light_source._specular_intensity = 1.2;
    light_source._diffuse_intensity = 5.0;
    light_source._attenuation_coeff = 0.0;
    
    // add the light to this apperance object
    apperance_0->addLightSource(light_source);
    
    
    GLSpotLightSource spotlight_source;
    spotlight_source._lightPos = glm::vec4(0.0,00.0,50.0, 1.0);
    spotlight_source._ambient_intensity = 0.0;
    spotlight_source._specular_intensity = 1.8;
    spotlight_source._diffuse_intensity = 2.0;
    spotlight_source._attenuation_coeff = 0.0002;
    spotlight_source._cone_direction = glm::vec3(0.0, 0.0,-1.0);
    spotlight_source._cone_angle = 10.0;
    
    apperance_0->addLightSource(spotlight_source);
    
    // Create a material object
    GLMaterial material_0;
    material_0._diffuse_material = glm::vec3(0.6, 0.0, 0.0);
    material_0._ambient_material = glm::vec3(0.6, 0.0, 0.0);
    material_0._specular_material = glm::vec3(1.0, 1.0, 1.0);
    material_0._shininess = 2.0;
    material_0._transparency = 1.0;
    
    // Add the material to the apperance object
    apperance_0->setMaterial(material_0);
    
    
    //************************************************************************************************
    // Add a texture
    GLTexture* texture = new GLTexture();
    texture->loadAndCreateTexture("../../data/textures/reflactance_map1.bmp");
    apperance_0->setTexture(texture);
    
    //************************************************************************************************
    // Finalize the appearance object
    apperance_0->finalize();
    
    
    // create the sphere geometry
    GLSphere3D* sphere_0 = new GLSphere3D(0.0,0.0,0.0, 10.0, 50, 50);
    sphere_0->setApperance(*apperance_0);
    sphere_0->init();
    
    // If you want to change appearance parameters after you init the object, call the update function
    apperance_0->updateLightSources();
    
    
    
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Main render loop
    
    // Set up our green background color
    static const GLfloat clear_color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    static const GLfloat clear_depth[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    // This sets the camera to a new location
    // the first parameter is the eye position, the second the center location, and the third the up vector.
    SetViewAsLookAt(glm::vec3(12.0f, 12.0f, 65.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    SetCameraManipulator(CAMERA_MANIPULATOR);
    
    
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
        
        
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //// This renders the objects
        
        // Set the trackball locatiom
        SetTrackballLocation(GetCurrentCameraMatrix(), GetCurrentCameraTranslation());
        
        // draw the objects
        cs->draw();
        sphere_0->draw();
 
        
        // change the texture appearance blend mode
        bool ret = texture->setTextureBlendMode(g_change_texture_blend);
        if(ret)apperance_0->updateTextures();
        
        
        //// This renders the objects
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        
        // Swap the buffers so that what we drew will appear on the screen.
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
    
    delete cs;
    
    
}

