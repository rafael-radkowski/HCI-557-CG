/*
Texture example.

This file demonstrate how to add a texture to an surface model in OpenGL and GLSL. 
The code creates a single plane and adds a texture to this plane. 

Note that this project incorporates multiple files:

-ModelPlane.h/.cpp: the plane including the texture coordinates were created in this file. 
	Study the function create to see how texture coordinates can be added to a vertex buffer object. 

-Texture2D.h./.cpp: this file (class) loads a texture from a bitmap file (.bmp) and creates an OpenGL/GLSL texture. 
	Look into CreateTexture2D(...) to see how the texture can be created. 

-BMPLoader.h/.cpp: this class contains a loader for a bitmap image. It is called within Texture2D. 

This file is part of CS/CPRE/ME 557 Computer Graphics at Iowa State University

Rafael Radkowski
Iowa State University
rafael@iastate.edu
+1 (515) 294-7044
MIT License

-------------------------------------------------------------------------------
Last edited:

Oct 20, 2019, RR:
- Added more comments
- Changed the shader loader and texture loader to seek the files if the path is incorrect. 

*/

// stl include
#include <iostream>
#include <string>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>  // transformation
#include <glm/gtx/quaternion.hpp> // quaternions

// glfw includes
#include <GLFW/glfw3.h>

// local
#define GLM_ENABLE_EXPERIMENTAL
#include "Window.h" // the windows
#include "OpenGLDefaults.h" // some open gl and glew defaults
#include "VertexBuffers.h"  // create vertex buffer object
#include "ShaderProgram.h"  // create a shader program
#include "Texture2D.h"


// Geometry
#include "ModelCoordinateSystem.h"
#include "ModelPlane.h"

using namespace std;
using namespace cs557;

//------------------------------------------------------------
//
//	Some global variables to keep track of

// The handle to the window object
GLFWwindow *window = NULL;

// Transformation pipeline variables
glm::mat4 projectionMatrix; // Variable to store the projection matrix
glm::mat4 viewMatrix;       // Variable to store the view matrix
glm::mat4 modelMatrix;      // Variable to store the model matrix for the plane. 


//------------------------------------------------------------
//
//	Some model  to keep track of

// a corodinate system
cs557::CoordinateSystem coordinateSystem;
glm::mat4 modelMatrixCoord; // Variable to store the model matrix for the coordinate system. . 

// The plane model. 
// This model includes vertices with normal vectors and texture coordinates. 
// Study Plane in ModelPlane.h/.cpp to see how it works. 
cs557::Plane plane0;


// Set up the background color and the clear color. 
GLfloat clear_color[] = {0.6f, 0.7f, 1.0f, 1.0f};
static const GLfloat clear_depth[] = {1.0f, 1.0f, 1.0f, 1.0f};


// This is the callback we'll be registering with GLFW for keyboard handling.
// The only thing we're doing here is setting up the window to close when we press ESC
void my_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   //cout << key ;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    else if(key == 84 && action == GLFW_PRESS) // t
    {
        
    }
    else if(key == 46 && action == GLFW_PRESS)
    {
        
    }
    else if(key == 44 && action == GLFW_PRESS)
    {
       
        
    }
    else if(key == 66 && action == GLFW_PRESS)
    {
    

    }


}





//------------------------------------------------------------
//
//	INIT THE SCENE

void Init(void)
{

    // Init the GLFW Window
    window = cs557::initWindow();

    // set a keyboard callback
    glfwSetKeyCallback(window, my_key_callback);

    // Initialize the GLEW apis
    cs557::initGlew();


    //-----------------------------------------------------------------------------------------------------------------------
	// Projection transformations
	// These lines define the major view pipeline. 
	projectionMatrix = glm::perspective(1.57f, (float)800 / (float)600, 0.1f, 100.f);
	modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); 
	viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, -4.f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  


    // create a coordinate system
    coordinateSystem.create(2.5);
	// Transformation for the coordinate system. It remains in the center. 
	modelMatrixCoord = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, 0.0f, 0.0f)); 


    // Load the shader program
	// This code loads a shader code from files and creates the OpenGL/GLSL shader program. 
	int texture_program = -1;
    texture_program = cs557::LoadAndCreateShaderProgram("../texture_program.vs", "../texture_program.fs");

    
    // create a plane
	// This line creates the plane of size 4 x 4. 
	// It also attaches the texture_program to the plane to become the active renderer. 
    plane0.create(4.0, 4.0, texture_program);


    // Load the texture image from a file and create the GLSL / OpenGL texture.
    // Note that this function binds the texture to texture unit GL_TEXTURE0 
    glUseProgram(texture_program);
    unsigned int texture_id = -1;
    LoadAndCreateTexture2D("../textures/texture_brick.bmp", &texture_id);

    // Activate the texture unit and bind the texture. 
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    // Fetch the texture location and set the parameter to 0.
    // Note that 0 is the number of the texture unit GL_TEXTURE0.
    int texture_location = glGetUniformLocation(texture_program, "tex");
    glUniform1i(texture_location, 0);

}

//------------------------------------------------------------
//
//	RENDER THE SCENE

void Draw(void)
{
	// This function initializes the camera with our view matrix. 
    cs557::InitControlsViewMatrix(viewMatrix);

    // Enable depth test
    glEnable(GL_DEPTH_TEST); // ignore this line

    while (!glfwWindowShouldClose(window))
    {

        // Clear the entire buffer with our green color (sets the background to be green).
        glClearBufferfv(GL_COLOR, 0, clear_color);
        glClearBufferfv(GL_DEPTH, 0, clear_depth);

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //// This renders the objects

        // update the camera values.
        // Note that this line changes the view matrix.
        glm::mat4 rotated_view = cs557::GetCamera().getRotationMatrix();

        //----------------------------------------------------------------------------------------------------------------------------
        // Object 0
        // This draws a coordinate system
        coordinateSystem.draw(projectionMatrix, rotated_view, modelMatrixCoord);



        // draw the plane
		// This function activates the related shader code, binds the texture and the geometry, 
		// and draws the geometry. 
        plane0.draw(projectionMatrix, rotated_view, modelMatrix);


        // Swap the buffers so that what we drew will appear on the screen.
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}








//------------------------------------------------------------
//
//	RENDER THE SCENE
int main(int argc, const char *argv[])
{
    cout << "This code example demonstrates texture mapping." << endl;
    cout << "Rafael Radkowski\nrafael@iastate.edu\nIowa State University" << endl;
    cout << "This example is part of ME/CS/CPRE557" << endl;



    Init();

    Draw();

    return 1;
}
