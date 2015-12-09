//
//  main_picking.cpp
//  HCI 557 Picking example
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
#include "Bezier.h"
#include "GLSurface.h"

using namespace std;


// The handle to the window object
GLFWwindow*         window;

/* A trackball to move and rotate the camera view */
extern Trackball trackball;







int main(int argc, const char * argv[])
{
    vector< vector<float> > control_points;
    vector< vector<float> > result;

    // These are the control points for a spline
    vector<float> p0 = vector<float>(3,0);
    p0[0] = 0.0; p0[1] = 1.0; p0[2] = 0.0;
    
    vector<float> p1 = vector<float>(3,0);
    p1[0] = 0.0; p1[1] = 2.0; p1[2] = 1.0;
    
    vector<float> p2 = vector<float>(3,0);
    p2[0] = 0.0; p2[1] = 3.0; p2[2] = 3.0;
    
    vector<float> p3 = vector<float>(3,0);
    p3[0] = 0.0; p3[1] = 2.0; p3[2] = 4.0;
    
    control_points.push_back(p0);
    control_points.push_back(p1);
    control_points.push_back(p2);
    control_points.push_back(p3);
    
    // This computes the spline and ten points along the spline.
    ComputeCubicSplineC(10, control_points, result);
    
    
    // Next, we create a surface with 4x4 control points.
    // A different way in order to introduce different computational way.s
    static const
    float data_surface[16][3] = {{0,0,0}, {1.5, 0, 0}, {2.2,0,0}, {4.0,0,0},
                                 {0,1,0.2}, {1.5, 1, 0.4}, {2.2,1,0.6}, {4.0,1,0.2},
                                 {0.1,2,0.2}, {1.4, 2, 0.5}, {2.0,2,0.8}, {3.9,2,0.2},
                                 {0.0,3,0.2}, {1.4, 3, 0.4}, {2.0,3,0.5}, {3.9,3,0.0}
                                 };
    
    
    vector< vector<float> > control_points_surface(16,vector<float>(3,0.0) );
    for(int i=0; i<16; i++){
     control_points_surface[i][0] = data_surface[i][0];
     control_points_surface[i][1] = data_surface[i][1];
     control_points_surface[i][2] = data_surface[i][2];
    }
    
    vector< vector<float> > result_surface;
    
    // This computes the points for this surface.
    ComputeCubicPatch(10, control_points_surface, result_surface );
    
    
    // In order to render the object, we need to vertices, triangles, and normals.
    // This function generates them for us.
    // glm::vec3(0.0,0.0,1.0) <- this is the general up direction of this surface.
    // This vector is used to align all the normal vectors. 
    vector< glm::vec3 > vertices;
    vector< glm::vec3 > normals;
    TriangulateCubicPatch(10, 10, result_surface, glm::vec3(0.0,0.0,1.0), vertices, normals );
    
    

    
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
    //GLAppearance* apperance_0 = new GLAppearance("../../data/shaders/multi_vertex_lights_ext.vs", "../../data/shaders/multi_vertex_lights.fs");
    GLAppearance* apperance_0 = new GLAppearance("../../data/shaders/multi_fragment_lights.vs", "../../data/shaders/multi_fragment_lights.fs");
    
    GLDirectLightSource  light_source;
    light_source._lightPos = glm::vec4(2.0,0.0,10.0, 0.0);
    light_source._ambient_intensity = 1.0;
    light_source._specular_intensity = 1.5;
    light_source._diffuse_intensity = 40.0;
    light_source._attenuation_coeff = 0.0;
    
    // add the light to this apperance object
    apperance_0->addLightSource(light_source);
    

    
    // Create a material object
    GLMaterial material_0;
    material_0._diffuse_material = glm::vec3(1.0, 0.0, 0.0);
    material_0._ambient_material = glm::vec3(1.0, 0.0, 0.0);
    material_0._specular_material = glm::vec3(1.0, 1.0, 1.0);
    material_0._shininess = 25.0;
    material_0._transparency = 1.0;
    
    // Add the material to the apperance object
    apperance_0->setMaterial(material_0);
    apperance_0->finalize();
    

    // If you want to change appearance parameters after you init the object, call the update function
    apperance_0->updateLightSources();
    
    

    
    GLSurface* surface = new GLSurface(vertices, normals);
    surface->setApperance(*apperance_0);
    surface->init();
    
    

    
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    
    
    // Set up our green background color
    static const GLfloat clear_color_white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    static const GLfloat clear_color_black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    static const GLfloat clear_depth[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    // This sets the camera to a new location
    // the first parameter is the eye position, the second the center location, and the third the up vector. 
    SetViewAsLookAt(glm::vec3(8.0f, 8.0f, 20.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    SetCameraDistance(15.0);
    
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
        glClearBufferfv(GL_COLOR , 0, clear_color_black);
        glClearBufferfv(GL_DEPTH , 0, clear_depth);
        
 
    
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //// This renders the objects
        
        // Set the trackball locatiom
        SetTrackballLocation(GetCurrentCameraMatrix(), GetCurrentCameraTranslation());
        
        // draw the objects
        cs->draw();
        surface->draw();
       
        //// This renders the objects
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        
        // Swap the buffers so that what we drew will appear on the screen.
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
    
    delete cs;


}

