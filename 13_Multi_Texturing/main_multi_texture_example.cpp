/*!


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

glm::mat4 projectionMatrix; // Store the projection matrix
glm::mat4 viewMatrix;       // Store the view matrix
glm::mat4 modelMatrix;      // Store the model matrix
glm::mat4 modelMatrixCoord;


//------------------------------------------------------------
//
//	Some model  to keep track of

// a corodinate system
cs557::CoordinateSystem coordinateSystem;

cs557::Plane plane0;


int texture_program = -1;

unsigned int texture_id0 = -1;
unsigned int texture_id1 = -1;


// Set up our green background color
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
	projectionMatrix = glm::perspective(1.57f, (float)800 / (float)600, 0.1f, 100.f);
	modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); 
	viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, -4.f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrixCoord = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, 0.0f, 0.0f)); 


    // create a coordinate system
    coordinateSystem.create(2.5);


    // Load the shader program
    texture_program = cs557::LoadAndCreateShaderProgram("../multi_texture_program.vs", "../multi_texture_program.fs");


    // create a plane
    plane0.create(4.0, 4.0, texture_program);


    // Load the texture and create it.
    // Note that this function bind the texture to texture unit GL_TEXTURE0 
    glUseProgram(texture_program);
    
    // This loads and activates the texture. 
    // Note that the texture remains activated after the entire process is ready.
    LoadAndCreateTexture2D("../textures/texture_brick.bmp", &texture_id0, GL_CLAMP_TO_BORDER,  GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);

    // Fetch the texture location and set the parameter to 0.
    // Note that 0 is the number of the texture unit GL_TEXTURE0.
    int texture_location = glGetUniformLocation(texture_program, "tex");
    glUniform1i(texture_location, 0);


    // Fetch the texture location and set the parameter to 1.
    // Note that 0 is the number of the texture unit GL_TEXTURE1.
    LoadAndCreateTexture2D("../textures/light_512_512a.bmp", &texture_id1, GL_CLAMP_TO_BORDER,  GL_TEXTURE1 );
    glEnable(GL_TEXTURE_2D);

    texture_location = glGetUniformLocation(texture_program, "tex_light_map");
    glUniform1i(texture_location, 1);


}

//------------------------------------------------------------
//
//	RENDER THE SCENE

void Draw(void)
{

    
    // Init the view matrix. 
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
        glm::mat4 rotated_view = cs557::GetCamera().getViewMatrix();

        //----------------------------------------------------------------------------------------------------------------------------
        // Object 0
        // This draws a coordinate system
        coordinateSystem.draw(projectionMatrix, rotated_view, modelMatrixCoord);

        // activate the units and bind the textures per unit. 
        // This makes sure that each unit has the right texture attached to it.
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture_id1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_id0);

        // draw the plane
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
