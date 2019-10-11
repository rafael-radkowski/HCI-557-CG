/*
main_lighting.cpp

This file introduces light and reflection models for OpenGL and GLSL
The class implements a per-fragment shader, which means that the light equations 
calcualte a light color per fragment (vs. per-vertex light, with one color per vertex only.)

The example renders a sphere and a box model. 
It includes one point light source. 
You can use the keyvboard keys to change the light and reflection parameters. 

Note that both light sources share the same material and the same shader code. 
The shader code for this file can be found in:
 - per_fragment_light.vs/.fs

 MIND THE LOCATION OF THE SHADER FILE. YOU MAY NEED TO CHANGE THE PATH. 

 The code has to main functions

- void Init(void): this function initializes the scene.
- void Draw(void): this function draw the scene. 


Rafael Radkowski
Iowa State University
November 2018
rafael@iastate.edu

MIT License
------------------------------------------------
Last changes:

Oct 10, 2019, RR
- Fixed a bug that caused a specular light artifact. 
- Added additional descriptions.

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

// Geometry
#include "ModelCoordinateSystem.h"
#include "ModelSphere.h"
#include "ModelBoxN.h"

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
glm::mat4 modelMatrix;      // Store the model matrix

// This matrix stores the position of the coordinate system. 
glm::mat4 modelMatrixCoord;      


//------------------------------------------------------------
//
//	Some model  to keep track of

// The coordinate system, which acts as a reference coordinate frame
// to indicate the global coordinate system. 
cs557::CoordinateSystem coordinateSystem;


// The sphere model.
// All geometry initializations, vertex buffer objects, etc. happen inside the member create. 
cs557::Sphere  sphere;

// A box model. 
// All geometry initializations, vertex buffer objects, etc. happen inside the member create. 
cs557::BoxN    box;

// A variable to store the glsl location for of shader program
int per_fragment_light_program = -1;



//---------------------------------------------------------
// Light model
// The light position defined as vector 3
glm::vec3   light_position;


// The camera position must be passed to the glsl shader program.
// This variable keeps the location of the light_position in the glsl program. 
int         light_shader_loc = -1;

//---------------------------------------------------------

// Material reflection intensity.
// These variables are defined on a global level so that the keyboard function 
// as well as the renderer can access them. 
float       diffuse_intensity = 0.2;
float       ambient_intensity = 0.5;
float       specular_intensity = 0.9;
float       shininess = 10.5;


//------------------------------------------------------------
//
//	Helpers

// This variable switches the user input mode. 
// Use 't' to switch between different paramters to set. 
int mode = 0;


// These are helper variables to switch on/off
// the reflection components. 
int diffuse_on = 1;
int specular_on = 1;
int ambient_on = 1;


// Set up our  background color and the depth buffer. 
GLfloat clear_color[] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat clear_depth[] = {1.0f, 1.0f, 1.0f, 1.0f};


// This is the callback we'll be registering with GLFW for keyboard handling.
// It catches all keyboard interaction and processes them. 
void my_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   //cout << key ;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    else if(key == 84 && action == GLFW_PRESS) // t
    {
        mode = mode + 1;
        if(mode >=4)mode = 0;

        switch(mode)
        {
            case 0:
            cout << "Diffuse intensity" << endl;
            break;
            case 1:
            cout << "Specular intensity" << endl;
            break;
            case 2:
            cout << "shininess" << endl;
            break;
            case 3:
            cout << "Ambient intensity" << endl;
            break;
        }
    }
    else if(key == 46 && action == GLFW_PRESS)
    {

        /*
        This part processes the user input and increases 
        the reflection intensity.
        */
        switch(mode)
        {
            case 0:
            diffuse_intensity += 0.05;
            if(diffuse_intensity > 10)diffuse_intensity = 10;
            break;
            case 1:
            specular_intensity += 0.05;
            if(specular_intensity > 10)specular_intensity = 10;
            break;
             case 2:
            shininess += 0.5;
            if(shininess > 150)shininess = 10;
            break;
            case 3:
             ambient_intensity += 0.05;
            if(ambient_intensity > 10)ambient_intensity = 10;
            break;
           
        }
    }
    else if(key == 44 && action == GLFW_PRESS)
    {
        /*
        This part processes the user input and increases 
        the reflection intensity.
        */
        switch(mode)
        {
            case 0:
            diffuse_intensity -= 0.05;
            if(diffuse_intensity < 0 )diffuse_intensity = 0;
            break;
            case 1:
            specular_intensity -= 0.05;
            if(specular_intensity < 0)specular_intensity = 0;
            break;
             case 2:
            shininess -= 0.5;
            if(shininess < 1) shininess = 1;
            break;
            case 3:
            ambient_intensity-= 0.05;
            if(ambient_intensity < 0)ambient_intensity = 0;
            break;
           
        }
    }
    else if(key == 66 && action == GLFW_PRESS)
    {
        if(clear_color[0] > 0.2 ){
            clear_color[0] = 0.0;
            clear_color[1] = 0.0;
            clear_color[2] = 0.0;
        }else{
            clear_color[0] = 0.6f;
            clear_color[1] = 0.7f;
            clear_color[2] = 1.0f;
        }

    }
    else if(key == 49 && action == GLFW_PRESS) // 1
    {   
        diffuse_on++;
        diffuse_on = diffuse_on %2;
        glUseProgram(per_fragment_light_program);
        glUniform1i(glGetUniformLocation(per_fragment_light_program, "diff_on"), diffuse_on);

    }
    else if(key == 50 && action == GLFW_PRESS) // 2
    {
        specular_on++;
        specular_on = specular_on%2;
        glUseProgram(per_fragment_light_program);
        glUniform1i(glGetUniformLocation(per_fragment_light_program, "spec_on"), specular_on);
    }
    else if(key == 51 && action == GLFW_PRESS) // 3
    {   
        ambient_on++;;
        ambient_on = ambient_on%2;
        glUseProgram(per_fragment_light_program);
        glUniform1i(glGetUniformLocation(per_fragment_light_program, "amb_on"), ambient_on);
    }

    if(action == GLFW_PRESS)
        cout << "Parameters: diff: " << diffuse_intensity  << " spec: " << specular_intensity << " shi: " << shininess << " amb: " << ambient_intensity << endl;
}





//------------------------------------------------------------
//
//	INIT THE SCENE

void Init(void)
{

    // Init the GLFW Window
    window = cs557::initWindow();

    // This sets the keyboard callback function
    // so that we can change teh light parameters interactively. 
    glfwSetKeyCallback(window, my_key_callback);

    // Initialize the GLEW apis
    cs557::initGlew();


    //-----------------------------------------------------------------------------------------------------------------------
	// The matrices for the projection pipeline
	projectionMatrix = glm::perspective(1.57f, (float)800 / (float)600, 0.1f, 100.f);
	modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f)); 
	viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, -7.f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    

    // This creates a 3D coordinate system to indicate the global origin of the 
    // scene. It is just a helper to show the (0,0,0) position of the world, and 
    // to have a frame of reference when moving the light. 
    coordinateSystem.create(2.5);

    // This is a helper model matrix to define the positition of the coordinate system mode. 
    modelMatrixCoord = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); 


    //-----------------------------------------------------------------------------------------------------------------------
	// Load a shader program from a file
    // This function is part of the file ShaderProgram.cpp/.h
    // It load the shader program from the file per_fragment_light.vs and per_fragment_light.fs
    // MIND THE PATH: YOU MAY HAVE TO CHANGE IT. 
    per_fragment_light_program = LoadAndCreateShaderProgram("../per_fragment_light.vs", "../per_fragment_light.fs");


    // This activates the shader program. 
    glUseProgram(per_fragment_light_program);

    // This line create a sphere model. 
    // See gl_common_ext/ModelSphere.h/.cpp for further details.
    sphere.create(1.0, 40, 40, per_fragment_light_program);

    // This line creates a box model. 
    // See gl_common_ext/ModelBoxN.h/.cpp for further details.
    box.create(1.5, 1.5, 1.5, per_fragment_light_program);

    //-----------------------------------------------------------------------------------------------------------------------
	// Material and light parameters. 
    // This section set the material and light parameters and uploads them to the shader program. 
    // Note that the shader MUST be loaded and used (using glUseProgram(per_fragment_light_program);) to get glUniform__ to work. 

    // The material model. 
    // This is a simple one with one color 
    glm::vec3 diff_color(1.0, 0.55, 0.05); // diffuse reflection
    glm::vec3 spec_color(1.0, 1.0, 1.0); // specular reflection
    glm::vec3 ambi_color(1.0, 0.55, 0.05); // ambient reflection
    diffuse_intensity = 1.25;//0.3f; // diffuse intensity
    ambient_intensity = 0.3;//0.55f;  // ambient intensity
    specular_intensity = 1.7f;  // specular intensity
    shininess = 14.0f;  // shininess exponent 
    
    // The light positon. This will become a spotlight. 
    glm::vec3 light_position(0.0, 6.0, -6.0);

    // This part set the variables values in the shader program. 
    // Not that this happens only once since it does not happen in the main loop. 
    // So updates are not possible. 
    glUniform3fv(glGetUniformLocation(per_fragment_light_program, "diffusecolor"), 1, &diff_color[0]);
    glUniform3fv(glGetUniformLocation(per_fragment_light_program, "specularColor"), 1, &spec_color[0]);
    glUniform3fv(glGetUniformLocation(per_fragment_light_program, "ambientcolor"), 1, &ambi_color[0]);
    glUniform3fv(glGetUniformLocation(per_fragment_light_program, "locationLight"), 1, &light_position[0]);
    glUniform1f(glGetUniformLocation(per_fragment_light_program, "diffuseIntensity"), diffuse_intensity);
    glUniform1f(glGetUniformLocation(per_fragment_light_program, "ambientIntensity"), ambient_intensity);
    glUniform1f(glGetUniformLocation(per_fragment_light_program, "specular_intensity"), specular_intensity);
    glUniform1f(glGetUniformLocation(per_fragment_light_program, "speculuar_coeff"), shininess);

   
    // This enables the reflection components. 
    // The code includes variable to allow one to enable or disable specific 
    // refelction components (use the keys 1, 2, 3). This ensures that all components are switched
    // on when starting the app. 
    glUniform1i(glGetUniformLocation(per_fragment_light_program, "diff_on"), 1.0);
    glUniform1i(glGetUniformLocation(per_fragment_light_program, "amb_on"), 1.0);
    glUniform1i(glGetUniformLocation(per_fragment_light_program, "spec_on"), 1.0);

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
        glm::mat4 rotated_view = cs557::GetCamera().getViewMatrix() * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f)) ;

        //----------------------------------------------------------------------------------------------------------------------------
        // Drawing

        // This draws a coordinate system
        coordinateSystem.draw(projectionMatrix, rotated_view, modelMatrixCoord);

        // These lines update the intensity values. 
        // It happens hear each frame, although it would not be necessary. 
        // One only need to update the shader when the values change. 
        // update values
        glUseProgram(per_fragment_light_program);
        glUniform1f(glGetUniformLocation(per_fragment_light_program, "diffuseIntensity"), diffuse_intensity);
        glUniform1f(glGetUniformLocation(per_fragment_light_program, "ambientIntensity"), ambient_intensity);
        glUniform1f(glGetUniformLocation(per_fragment_light_program, "specular_intensity"), specular_intensity);
        glUniform1f(glGetUniformLocation(per_fragment_light_program, "speculuar_coeff"), shininess);

        // draw the sphere
        sphere.draw(projectionMatrix, rotated_view, modelMatrix);

        // draw the box. 
        box.draw(projectionMatrix, rotated_view, glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.0f, 0.0f)));

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
    cout << "This code example demonstrates a light source." << endl;
    cout << "Rafael Radkowski\nrafael@iastate.edu\nIowa State University" << endl;
    cout << "This example is part of ME/CS/CPRE557" << endl;

    cout << "\nUse the following keys to change the light" <<endl;
    cout << "\t't' - toggles between diffuce, specular, ambient, and shininess parameters " << endl;
    cout << "\t'.' - increase the value" << endl;
    cout << "\t',' - decrease the value" << endl;
    cout << "\nTurn on/off reflection components using the keys:" << endl;
    cout << "\t'1' - diffuse reflection" << endl;
    cout << "\t'2' - specular reflection" << endl;
    cout << "\t'3' - ambient reflection" << endl;

    Init();

    Draw();

    return 1;
}
