/*
main_pbr.cpp

This file demonstrates the implementation of a PBR using a  direct light BRDF


The scene shows for sphere with ugly material. The spheres are declared as
- sphere_01
- sphere_02
- sphere_03
- sphere_04

Each sphere comes with different BRDF settings, from rough to metallic. 


Rafael Radkowski
Iowa State University
November 2018
rafael@iastate.edu

MIT License
------------------------------------------------
Last changes:


*/
// stl include
#include <iostream>
#include <string>
#include <stdio.h>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>  // transformation
#include <glm/gtx/quaternion.hpp> // quaternions

// glfw includes
#include <GLFW/glfw3.h>

// local
#include "Window.h" // the windows
#include "OpenGLDefaults.h" // some open gl and glew defaults
#include "VertexBuffers.h"  // create vertex buffer object
#include "ShaderProgram.h"  // create a shader program
#include "CommonTypes.h"  // light and material definitions
#include "BRDFTypes.h" // The brdf material
#include "BRDFLoader.h"


// Geometry
#include "ModelCoordinateSystem.h"
#include "ModelSphere.h"
#include "ModelBoxN.h"
#include "ModelOBJ.h"

using namespace std;
using namespace cs557;



//------------------------------------------------------------
//
//	Some global variables to keep track of

// The handle to the window object
GLFWwindow *window = NULL;


// Transformation pipeline variables
glm::mat4 projectionMatrix; // Store the projection matrix
glm::mat4 viewMatrix;       // Store the view matrix

// This matrix stores the position of the coordinate system. 
glm::mat4 modelMatrixCoord;      


//------------------------------------------------------------
//
//	Some model  to keep track of

// The coordinate system, which acts as a reference coordinate frame
// to indicate the global coordinate system. 
cs557::CoordinateSystem coordinateSystem;


// The sphere model.
// All geometry initializations, vertex buffer objects, etc.
cs557::OBJModel  sphere_01;
cs557::OBJModel  sphere_02;
cs557::OBJModel  sphere_03;
cs557::OBJModel  sphere_04;

cs557::OBJModel  sphere_11;
cs557::OBJModel  sphere_12;
cs557::OBJModel  sphere_13;
cs557::OBJModel  sphere_14;

// A variable to store the glsl location for 
// all the shader programs, one for each sphere. 
int program_sphere_01 = -1;
int program_sphere_02 = -1;
int program_sphere_03 = -1;
int program_sphere_04 = -1;
int program_sphere_11 = -1;
int program_sphere_12 = -1;
int program_sphere_13 = -1;
int program_sphere_14 = -1;

// The light source
LightSource     light0;
LightSource     light1;
LightSource     light2;



BRDFTexMaterial brdf_tex0;
BRDFTexMaterial brdf_tex1;
BRDFTexMaterial brdf_tex2;
BRDFTexMaterial brdf_tex3;
BRDFTexMaterial brdf_tex4;
BRDFTexMaterial brdf_tex5;
BRDFTexMaterial brdf_tex6;
BRDFTexMaterial brdf_tex7;

// Set up our  background color and the depth buffer. 
GLfloat clear_color[] = {0.0f, 0.0f, 0.0f, 0.0f};
GLfloat clear_depth[] = {1.0f, 1.0f, 1.0f, 1.0f};






 /*
Create light sources
Light sources on the host system are just parameters that get passed
to the shader programs to fed the light and reflection models. 
You can find the light source and material type declarations 
in CommonTypes.h.
Both types come with a method: setAllUniform(int program),
which passes all type variables as uniform variables to the program. 
*/
void CreateLights(void)
{
    //---------------------------------------------------------
    // Create a first light.
    light0.index = 0;
    light0.pos =  glm::vec3(0.0f, 6.0f, -16.0f);
    light0.dir =  glm::vec3(0.0f, 0.0f, 0.0f);
    light0.k1 = 0.1;
    light0.intensity = 1.0;

    //---------------------------------------------------------
    // Create a second light. 
    light1.index = 1;
    light1.pos =  glm::vec3(0.0f, -6.0f, -16.0f);
    light1.dir =  glm::vec3(0.0f, 0.0f, 0.0f);
    light1.color = glm::vec3(0.0f, 0.0f, 1.0f);
    light1.intensity = 1.0;
}






//------------------------------------------------------------
//
//	INIT THE SCENE
void Init(void)
{

    // Init the GLFW Window
    window = cs557::initWindow();


    // Initialize the GLEW apis
    cs557::initGlew();


    //-------------------------------------------------------------------------
	// The matrices for the projection pipeline
	projectionMatrix = glm::perspective(0.6f, (float)800 / (float)600, 0.1f, 100.f);
	viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, -20.f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    

    // This creates a 3D coordinate system to indicate the global origin of the 
    // scene. It is just a helper to show the (0,0,0) position of the world, and 
    // to have a frame of reference when moving the light. 
    coordinateSystem.create(2.5);

    // This is a helper model matrix to define the positition of the coordinate system mode. 
    modelMatrixCoord = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); 


    //----------------------------------------------------------------------
	// Load a shader program from a file
    program_sphere_01 = LoadAndCreateShaderProgram("../brdf_tex_shader.vs", "../brdf_tex_shader.fs");
    program_sphere_02 = LoadAndCreateShaderProgram("../brdf_tex_shader.vs", "../brdf_tex_shader.fs");
    program_sphere_03 = LoadAndCreateShaderProgram("../brdf_tex_shader.vs", "../brdf_tex_shader.fs");
    program_sphere_04 = LoadAndCreateShaderProgram("../brdf_tex_shader.vs", "../brdf_tex_shader.fs");
    program_sphere_11 = LoadAndCreateShaderProgram("../brdf_tex_shader.vs", "../brdf_tex_shader.fs");
    program_sphere_12 = LoadAndCreateShaderProgram("../brdf_tex_shader.vs", "../brdf_tex_shader.fs");
    program_sphere_13 = LoadAndCreateShaderProgram("../brdf_tex_shader.vs", "../brdf_tex_shader.fs");
    program_sphere_14 = LoadAndCreateShaderProgram("../brdf_tex_shader.vs", "../brdf_tex_shader.fs");


    // This line create a sphere model. 
    // See gl_common_ext/ModelSphere.h/.cpp for further details.
    sphere_01.create("../sphere_t.obj", program_sphere_01);
    sphere_02.create("../sphere_t.obj", program_sphere_02);
    sphere_03.create("../sphere_t.obj", program_sphere_03);
    sphere_04.create("../sphere_t.obj", program_sphere_04);

    sphere_11.create("../sphere_t.obj", program_sphere_11);
    sphere_12.create("../sphere_t.obj", program_sphere_12);
    sphere_13.create("../sphere_t.obj", program_sphere_13);
    sphere_14.create("../sphere_t.obj", program_sphere_14);


    //--------------------------------------------------------------------------
	// Material and light parameters. 


    // Create the lights
    CreateLights();

    light0.apply(sphere_01.getProgram());
    light0.apply(sphere_02.getProgram());
    light0.apply(sphere_03.getProgram());
    light0.apply(sphere_04.getProgram());
    light0.apply(sphere_11.getProgram());
    light0.apply(sphere_12.getProgram());
    light0.apply(sphere_13.getProgram());
    light0.apply(sphere_14.getProgram());
    light1.apply(sphere_01.getProgram());
    light1.apply(sphere_02.getProgram());
    light1.apply(sphere_03.getProgram());
    light1.apply(sphere_04.getProgram());
    light1.apply(sphere_11.getProgram());
    light1.apply(sphere_12.getProgram());
    light1.apply(sphere_13.getProgram());
    light1.apply(sphere_14.getProgram());
    




    BRDFLoader::ReadBMP(brdf_tex0,  "../PBR/layered-rock1/layered-rock1-albedo.bmp",
                                    "../PBR/layered-rock1/layered-rock1-Metalness.bmp",
                                    "../PBR/layered-rock1/layered-rock1-rough.bmp",
                                     "../PBR/layered-rock1/layered-rock1-ao.bmp");

    brdf_tex0.lightColor = glm::vec3(53.47, 51.31, 50.79);
    brdf_tex0.F0 = glm::vec3(0.04, 0.04, 0.04);
    brdf_tex0.k1 = 0.1;
    brdf_tex0.k2 = 0.1;
    brdf_tex0.apply(sphere_01.getProgram());


    BRDFLoader::ReadBMP(brdf_tex1,  "../PBR/chipped-paint-metal/chipped-paint-metal-albedo.bmp",
                                    "../PBR/chipped-paint-metal/chipped-paint-metal-metal.bmp",
                                    "../PBR/chipped-paint-metal/chipped-paint-metal-rough2.bmp",
                                     "../PBR/chipped-paint-metal/chipped-paint-ao.bmp");
    

    brdf_tex1.lightColor = glm::vec3(53.47, 51.31, 50.79);
    brdf_tex1.F0 = glm::vec3(0.04, 0.04, 0.04);
    brdf_tex1.k1 = 0.1;
    brdf_tex1.k2 = 0.1;
    brdf_tex1.apply(sphere_02.getProgram());



    BRDFLoader::ReadBMP(brdf_tex2,  "../PBR/bamboo-wood-semigloss/bamboo-wood-semigloss-albedo.bmp",
                                    "../PBR/bamboo-wood-semigloss/bamboo-wood-semigloss-metal.bmp",
                                    "../PBR/bamboo-wood-semigloss/bamboo-wood-semigloss-roughness.bmp",
                                     "../PBR/bamboo-wood-semigloss/bamboo-wood-semigloss-ao.bmp");
    

    brdf_tex2.lightColor = glm::vec3(53.47, 51.31, 50.79);
    brdf_tex2.F0 = glm::vec3(0.25, 0.25, 0.25);
    brdf_tex2.k1 = 0.05;
    brdf_tex2.k2 = 0.02;
    brdf_tex2.apply(sphere_03.getProgram());




    BRDFLoader::ReadBMP(brdf_tex3,  "../PBR/rustediron2/rustediron2_basecolor.bmp",
                                    "../PBR/rustediron2/rustediron2_metallic.bmp",
                                    "../PBR/rustediron2/rustediron2_roughness.bmp",
                                    "../PBR/rustediron2/rustediron2_ao.bmp");
    

    brdf_tex3.lightColor = glm::vec3(53.47, 51.31, 54.79);
    brdf_tex3.F0 = glm::vec3(0.06, 0.07, 0.08);
    brdf_tex3.k1 = 0.05;
    brdf_tex3.k2 = 0.02;
    brdf_tex3.apply(sphere_04.getProgram());



    BRDFLoader::ReadBMP(brdf_tex4, "../PBR/streaked_metal1/streaked-metal1-albedo.bmp",
                                    "../PBR/streaked_metal1/streaked-metal1-metalness.bmp",
                                    "../PBR/streaked_metal1/streaked-metal1-roughness.bmp",
                                    "../PBR/streaked_metal1/streaked-metal1-ao.bmp");
    

    brdf_tex4.lightColor = glm::vec3(73.47, 71.31, 74.79);
    brdf_tex4.F0 = glm::vec3(0.86, 0.87, 0.88);
    brdf_tex4.k1 = 0.05;
    brdf_tex4.k2 = 0.02;
    brdf_tex4.apply(sphere_11.getProgram());
  
  
 


}

//------------------------------------------------------------
//
//	RENDER THE SCENE
void Draw(void)
{

     // Init the view matrix. 
     // This is a helper for the camera control.
     // It set the camera control to the initial position as defined in the view matrix. 
    cs557::InitControlsViewMatrix(viewMatrix);
    

    // Enable depth test
    glEnable(GL_DEPTH_TEST); 

    while (!glfwWindowShouldClose(window))
    {

        // Clear the entire buffer with our green color (sets the background to be green).
        glClearBufferfv(GL_COLOR, 0, clear_color);
        glClearBufferfv(GL_DEPTH, 0, clear_depth);

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //// This renders the objects

        // update the camera values.
        // Note that this line changes the view matrix.
        // It adapts the view matrix depending on the user's input. 
        glm::mat4 rotated_view = cs557::GetCamera().getViewMatrix();

        //----------------------------------------------------------------------------------------------------------------------------
        // Drawing

        // This draws a coordinate system
        coordinateSystem.draw(projectionMatrix, rotated_view, modelMatrixCoord);

        // draw the spheres

        brdf_tex0.activateMaps(sphere_01.getProgram());
        sphere_01.draw(projectionMatrix, rotated_view, glm::translate(glm::mat4(1.0f), glm::vec3(4.5f, 2.0f, 0.0f)) );

        brdf_tex1.activateMaps(sphere_02.getProgram());
        sphere_02.draw(projectionMatrix, rotated_view, glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, 2.0f, 0.0f)) );

        brdf_tex2.activateMaps(sphere_03.getProgram());
        sphere_03.draw(projectionMatrix, rotated_view, glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, 2.0f, 0.0f)) );
        
        brdf_tex3.activateMaps(sphere_04.getProgram());
        sphere_04.draw(projectionMatrix, rotated_view, glm::translate(glm::mat4(1.0f), glm::vec3(-4.5f, 2.0f, 0.0f)) );

        brdf_tex4.activateMaps(sphere_11.getProgram());
        sphere_11.draw(projectionMatrix, rotated_view, glm::translate(glm::mat4(1.0f), glm::vec3(4.5f, -2.0f, 0.0f)) );
      //  sphere_12.draw(projectionMatrix, rotated_view, glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, -2.0f, 0.0f)) );
      //  sphere_13.draw(projectionMatrix, rotated_view, glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f,-2.0f, 0.0f)) );
      //  sphere_14.draw(projectionMatrix, rotated_view, glm::translate(glm::mat4(1.0f), glm::vec3(-4.5f, -2.0f, 0.0f)) );
    

        // Swap the buffers so that what we drew will appear on the screen.
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}








//------------------------------------------------------------
// The main program 
// 
int main(int argc, const char *argv[])
{
    cout << "Homework 4 - Light and Materials." << endl;
    cout << "Rafael Radkowski\nrafael@iastate.edu\nIowa State University" << endl;
    cout << "This example is part of ME/CS/CPRE557" << endl;

    Init();

    Draw();

    return 1;
}
