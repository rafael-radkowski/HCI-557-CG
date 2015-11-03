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
#include "Sphere3D.h"


#include "GLObjectObj.h"
#include "RayIntersectionTest.h"



using namespace std;


// The handle to the window object
GLFWwindow*         window;

// Define some of the global variables we're using for this sample
GLuint program;

/* A trackball to move and rotate the camera view */
extern Trackball trackball;


// The sphere object
GLSphere3D* sphere;
GLSphere3D* sphere_result;

// The loaded object
GLObjectObj* loadedModel1;

// to transform the sphere
glm::mat4 g_tranform_sphere;
glm::mat4 g_tranform_sphere_result;

float g_delta = 0.4;










// This is the callback we'll be registering with GLFW for keyboard handling.
// The only thing we're doing here is setting up the window to close when we press ESC
void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    bool move = false;
    
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    else if(key == 87 && action == GLFW_REPEAT) // key w
    {
        //cout <<  "key w pressed" << endl;
        g_tranform_sphere =  g_tranform_sphere * glm::translate(glm::vec3(0.0, g_delta, 0.0f));
        sphere->setMatrix(g_tranform_sphere);
        move = true;
    }
    else if(key == 65 && action == GLFW_REPEAT) // key a
    {
        //cout <<  "key a pressed" << endl;
        g_tranform_sphere =  g_tranform_sphere * glm::translate(glm::vec3(-g_delta,0.0 , 0.0f));
        sphere->setMatrix(g_tranform_sphere);
        move = true;
    }
    else if(key == 83 && action == GLFW_REPEAT) // key s
    {
        //cout <<  "key s pressed" << endl;
        g_tranform_sphere =  g_tranform_sphere * glm::translate(glm::vec3(0.0, -g_delta, 0.0f));
        sphere->setMatrix(g_tranform_sphere);
        move = true;
    }
    else if(key == 68 && action == GLFW_REPEAT) // key d
    {
        //cout <<  "key d pressed" << endl;
        g_tranform_sphere =  g_tranform_sphere * glm::translate(glm::vec3(g_delta,0.0, 0.0f));
        sphere->setMatrix(g_tranform_sphere);
        move = true;
    }
    
    
    if(move)
    {
        glm::vec3 s(g_tranform_sphere[3][0],g_tranform_sphere[3][1],g_tranform_sphere[3][2]);
        glm::vec3 e(g_tranform_sphere[3][0],g_tranform_sphere[3][1],g_tranform_sphere[3][2]-20);
        vector<glm::vec3> res;
        
        
        // perform the ray intersectiont test.
        RayIntersectionTest::intersect(s, e, *loadedModel1, res);
        
        // pick the first result if one is available
        if (res.size() > 0) {
            glm::vec3 position = res[0];
            
            g_tranform_sphere_result = glm::translate(position);
            sphere_result->setMatrix(g_tranform_sphere_result);
        }
    
       // cout << key << endl;
    }
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
    //// Create the underground model
    
    // coordinate system
    CoordSystem* cs = new CoordSystem(40.0);
    
    
    // create an apperance object.
    GLAppearance* apperance_0 = new GLAppearance("../../data/shaders/multi_vertex_lights.vs", "../../data/shaders/multi_vertex_lights.fs");
    
    GLDirectLightSource  light_source;
    light_source._lightPos = glm::vec4(20.0,20.0,0.0, 0.0);
    light_source._ambient_intensity = 0.2;
    light_source._specular_intensity = 0.5;
    light_source._diffuse_intensity = 2.0;
    light_source._attenuation_coeff = 0.0;
    
    // add the light to this apperance object
    apperance_0->addLightSource(light_source);
    
    // Create a material object
    GLMaterial material_0;
    material_0._diffuse_material = glm::vec3(0.2, 1.0, 0.2);
    material_0._ambient_material = glm::vec3(0.0, 1.0, 0.0);
    material_0._specular_material = glm::vec3(1.0, 1.0, 1.0);
    material_0._shininess = 2.0;
    material_0._transparency = 1.0;
    
    // Add the material to the apperance object
    apperance_0->setMaterial(material_0);
    apperance_0->finalize();
    

    // If you want to change appearance parameters after you init the object, call the update function
    apperance_0->updateLightSources();

    
    loadedModel1 = new GLObjectObj("../../data/height_map_t.obj");
    loadedModel1->setApperance(*apperance_0);
    loadedModel1->init();
    
    // This scales the underground model
    glm::mat4 tranform = glm::scale(glm::vec3(10.0, 10.0f, 10.0f));
    loadedModel1->setMatrix(tranform);
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Create the sphere
    
    
    GLAppearance* apperance_sphere = new GLAppearance("../../data/shaders/multi_vertex_lights.vs", "../../data/shaders/multi_vertex_lights.fs");
    // add the light to this apperance object
    apperance_sphere->addLightSource(light_source);
    

    // Create a material object
    GLMaterial material_sphere;
    material_sphere._diffuse_material = glm::vec3(1.0, 0.0, 0.0);
    material_sphere._ambient_material = glm::vec3(1.0, 0.0, 0.0);
    material_sphere._specular_material = glm::vec3(1.0, 1.0, 1.0);
    material_sphere._shininess = 2.0;
    material_sphere._transparency = 1.0;
    
    // Add the material to the apperance object
    apperance_sphere->setMaterial(material_sphere);
    apperance_sphere->finalize();

    // Create a sphere
    sphere = new GLSphere3D(0.0, 0.0, 0.0, 0.5);
    sphere->setApperance(*apperance_sphere);
    sphere->init();
    
    // move the sphere up
    g_tranform_sphere = glm::translate(glm::vec3(0.0, 0.0f, 10.0f));
    sphere->setMatrix(g_tranform_sphere);
    
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Create another sphere to indicate the results
    
    GLAppearance* apperance_sphere2 = new GLAppearance("../../data/shaders/multi_vertex_lights.vs", "../../data/shaders/multi_vertex_lights.fs");
    // add the light to this apperance object
    apperance_sphere2->addLightSource(light_source);
    
    
    // Create a material object
    GLMaterial material_sphere2;
    material_sphere2._diffuse_material = glm::vec3(1.0, 1.0, 0.0);
    material_sphere2._ambient_material = glm::vec3(1.0, 1.0, 0.0);
    material_sphere2._specular_material = glm::vec3(1.0, 1.0, 1.0);
    material_sphere2._shininess = 2.0;
    material_sphere2._transparency = 1.0;
    
    // Add the material to the apperance object
    apperance_sphere2->setMaterial(material_sphere2);
    apperance_sphere2->finalize();
    
    sphere_result = new GLSphere3D(0.0, 0.0, 0.0, 0.5);
    sphere_result->setApperance(*apperance_sphere2);
    sphere_result->init();
    
    
    g_tranform_sphere_result = glm::translate(glm::vec3(0.0, 0.0f, 5.0f));
    sphere_result->setMatrix(g_tranform_sphere_result);
    
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
    //// Here we set a new keyboard callback
    
    // Set the keyboard callback so that when we press ESC, it knows what to do.
    glfwSetKeyCallback(window, keyboard_callback);
    
    
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
        
        loadedModel1->draw();
        sphere->draw();
        sphere_result->draw();
        
        
        


        
       
        //// This renders the objects
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        
        // Swap the buffers so that what we drew will appear on the screen.
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
    
    delete cs;


}

