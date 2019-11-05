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

// the sphere material
Material	 	material;

BRDFMaterial    brdf0;
BRDFMaterial    brdf1;
BRDFMaterial    brdf2;
BRDFMaterial    brdf3;

BRDFMaterial    brdf11;
BRDFMaterial    brdf12;
BRDFMaterial    brdf13;
BRDFMaterial    brdf14;

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
    light0.pos =  glm::vec3(0.0f, 4.0f, -4.0f);
    light0.dir =  glm::vec3(0.0f, 0.0f, 0.0f);
    light0.k1 = 0.1;
    light0.intensity = 1.0;

    //---------------------------------------------------------
    // Create a second light. 
    light1.index = 1;
    light1.pos =  glm::vec3(0.0f, -4.0f, -4.0f);
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
    program_sphere_01 = LoadAndCreateShaderProgram("../brdf_shader.vs", "../brdf_shader.fs");
    program_sphere_02 = LoadAndCreateShaderProgram("../brdf_shader.vs", "../brdf_shader.fs");
    program_sphere_03 = LoadAndCreateShaderProgram("../brdf_shader.vs", "../brdf_shader.fs");
    program_sphere_04 = LoadAndCreateShaderProgram("../brdf_shader.vs", "../brdf_shader.fs");
    program_sphere_11 = LoadAndCreateShaderProgram("../brdf_shader.vs", "../brdf_shader.fs");
    program_sphere_12 = LoadAndCreateShaderProgram("../brdf_shader.vs", "../brdf_shader.fs");
    program_sphere_13 = LoadAndCreateShaderProgram("../brdf_shader.vs", "../brdf_shader.fs");
    program_sphere_14 = LoadAndCreateShaderProgram("../brdf_shader.vs", "../brdf_shader.fs");


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




    brdf0.albedo = glm::vec3(1.0, 0.0, 0.0);
    brdf0.lightColor = glm::vec3(23.47, 21.31, 20.79);
    //brdf0.F0 = glm::vec3(0.04, 0.04, 0.04);
    brdf0.roughness = 0.3f;
    brdf0.metallic = 0.0f;
    brdf0.ao = 1.0f;
    brdf0.k1 = 0.1;
    brdf0.k2 = 0.1;
    brdf0.apply(sphere_01.getProgram());

    brdf1.albedo = glm::vec3(1.0, 0.0, 0.0);
    brdf1.lightColor = glm::vec3(23.47, 21.31, 20.79);
    brdf1.roughness = 0.3f;
    brdf1.metallic = 0.33f;
    brdf1.ao = 1.0f;
    brdf1.k1 = 0.1;
    brdf1.k2 = 0.1;
    brdf1.apply(sphere_02.getProgram());


    brdf2.albedo = glm::vec3(1.0, 0.0, 0.0);
    brdf2.lightColor = glm::vec3(23.47, 21.31, 20.79);
    brdf2.roughness = 0.3f;
    brdf2.metallic = 0.66f;
    brdf2.ao = 1.0f;
    brdf2.k1 = 0.1;
    brdf2.k2 = 0.1;
    brdf2.apply(sphere_03.getProgram());

    brdf3.albedo = glm::vec3(1.0, 0.0, 0.0);
    brdf3.lightColor = glm::vec3(23.47, 21.31, 20.79);
    brdf3.roughness = 0.3f;
    brdf3.metallic = 0.9f;
    brdf3.ao = 1.0f;
    brdf3.k1 = 0.1;
    brdf3.k2 = 0.1;
    brdf3.apply(sphere_04.getProgram());

    // second raow


    brdf11.albedo = glm::vec3(1.0, 0.0, 0.0);
    brdf11.lightColor = glm::vec3(23.47, 21.31, 20.79);
    //brdf11.F0 = glm::vec3(0.04, 0.04, 0.04);
    brdf11.roughness = 0.6f;
    brdf11.metallic = 0.0f;
    brdf11.ao = 1.0f;
    brdf11.k1 = 0.1;
    brdf11.k2 = 0.1;
    brdf11.apply(sphere_11.getProgram());

    brdf12.albedo = glm::vec3(1.0, 0.0, 0.0);
    brdf12.lightColor = glm::vec3(23.47, 21.31, 20.79);
    brdf12.roughness = 0.6f;
    brdf12.metallic = 0.33f;
    brdf12.ao = 1.0f;
    brdf12.k1 = 0.1;
    brdf12.k2 = 0.1;
    brdf12.apply(sphere_12.getProgram());


    brdf13.albedo = glm::vec3(1.0, 0.0, 0.0);
    brdf13.lightColor = glm::vec3(23.47, 21.31, 20.79);
    brdf13.roughness = 0.6f;
    brdf13.metallic = 0.66f;
    brdf13.ao = 1.0f;
    brdf13.k1 = 0.1;
    brdf13.k2 = 0.1;
    brdf13.apply(sphere_13.getProgram());

    brdf14.albedo = glm::vec3(1.0, 0.0, 0.0);
    brdf14.lightColor = glm::vec3(23.47, 21.31, 20.79);
    brdf14.roughness = 0.6f;
    brdf14.metallic = 0.9f;
    brdf14.ao = 1.0f;
    brdf14.k1 = 0.1;
    brdf14.k2 = 0.1;
    brdf14.apply(sphere_14.getProgram());


  


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
        sphere_01.draw(projectionMatrix, rotated_view, glm::translate(glm::mat4(1.0f), glm::vec3(4.5f, 2.0f, 0.0f)) );
        sphere_02.draw(projectionMatrix, rotated_view, glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, 2.0f, 0.0f)) );
        sphere_03.draw(projectionMatrix, rotated_view, glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, 2.0f, 0.0f)) );
        sphere_04.draw(projectionMatrix, rotated_view, glm::translate(glm::mat4(1.0f), glm::vec3(-4.5f, 2.0f, 0.0f)) );

        // draw the spheres
        sphere_11.draw(projectionMatrix, rotated_view, glm::translate(glm::mat4(1.0f), glm::vec3(4.5f, -2.0f, 0.0f)) );
        sphere_12.draw(projectionMatrix, rotated_view, glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, -2.0f, 0.0f)) );
        sphere_13.draw(projectionMatrix, rotated_view, glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f,-2.0f, 0.0f)) );
        sphere_14.draw(projectionMatrix, rotated_view, glm::translate(glm::mat4(1.0f), glm::vec3(-4.5f, -2.0f, 0.0f)) );
    

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
