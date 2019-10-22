/*

Multi-Texture example.

This example demonstrates how one can combine two textures and use them in a fragment shader.
The scene renders a simple plane with two textures attached to it. 
The first texture is a brick texture (texture_brick.bmp), and the second texture is a light map 
(light_512_512a.bmp).
The background texture is linked to texture unit 0 and the light texture to texture unit 1. 

This example includes multiple files:

- Texture2D.h/.cpp: the entire texturing process happens in this file. Study the code to understand it. 

- ModelPlane.h/.cpp: defines and creates the plane, including the vertices, normal vectors, and texture coordinates. 

- multi_texture_program.fs/.vs: the shader code for this example. The entire texturing process happens in this program. 

Rafael Radkowski
Iowa State University
rafael@iastate.edu
+1 (515) 294-7044
MIT License

-------------------------------------------------------------------------------
Last edited:


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



//------------------------------------------------------------
//
//	Some model  to keep track of

// a corodinate system
cs557::CoordinateSystem coordinateSystem; // coordinate system geometry
glm::mat4 modelMatrixCoord;  // a model matrix for the coordinate system. 

// the background plane. 
cs557::Plane plane0;

// a variable to store the shader program id. 
int texture_program = -1;

// two variables to store the texture ids.
unsigned int texture_id0 = -1;
unsigned int texture_id1 = -1;


// Set up our green background color
GLfloat clear_color[] = {0.6f, 0.7f, 1.0f, 1.0f};
static const GLfloat clear_depth[] = {1.0f, 1.0f, 1.0f, 1.0f};

// keyboard helper variables. 
int tex0 = 1;
int tex1 = 1;

// This is the callback we'll be registering with GLFW for keyboard handling.
// The only thing we're doing here is setting up the window to close when we press ESC
void my_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   //cout << key ;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    else if(key == 49 && action == GLFW_PRESS) // 1
    {
        tex0 = ++tex0%2;
    }
    else if(key == 50 && action == GLFW_PRESS) // 2
    {
        tex1 = ++tex1%2;
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
	viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, -3.f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    

    // create a coordinate system
    // This coordinate system just acts as a reference system to keep track of the position and orientation of the scene. 
    coordinateSystem.create(2.5);
    modelMatrixCoord = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, 0.0f, 0.0f)); 


    // Load the shader program
    // This line loads the shader programs from two files and creates & links the program. 
    // Study the related function in ShaderProgram.h/.cpp to understand the details. 
    texture_program = cs557::LoadAndCreateShaderProgram("../multi_texture_program.vs", "../multi_texture_program.fs");


    // Create a plane.
    // This line creates the plane object including the vertices, normal vectors, and texture coordinates. 
    // It also creates the vertex buffer objects and links the content to shader variables. 
    plane0.create(4.0, 4.0, texture_program);


    // Enable the texture program. 
    // Thus, all functions affect this program. 
    glUseProgram(texture_program);


    // This line enables the texture procssing capability. 
    // Note that removing the line may not have an effect since LoadAndCreateTexture2D(..)
    // also eblables the unit internally. 
    // However, this is just a reminder that the capability must be enabled to use texture
    glEnable(GL_TEXTURE_2D);
    

    // Load and create the first texture.  
    // The function is implemented in Texture2D.h/.cpp. Study the code. 
    // The function loads a texture from a file
    // The second variable points to the location to store the texture id. 
    // The third variable defines the clamp mode, and the last variable the texture unit. 
    // Note that texture unit 0 is used in this case. 
    LoadAndCreateTexture2D("../textures/texture_brick.bmp", &texture_id0, GL_CLAMP_TO_BORDER,  GL_TEXTURE0);

    // The shader program uses the variable 'tex' to address the texture content. 
    // Fetch the variable location and set the parameter to 0.
    // Note that 0 is the number of the texture unit GL_TEXTURE0.
    int texture_location = glGetUniformLocation(texture_program, "tex");
    glUniform1i(texture_location, 0);


    // Load and create the second texture.  
    // Note that 0 is the number of the texture unit GL_TEXTURE1.
    LoadAndCreateTexture2D("../textures/light_512_512a.bmp", &texture_id1, GL_CLAMP_TO_BORDER,  GL_TEXTURE1 );

    // The shader program uses the variable 'ttex_light_mapex' to address the light map content. 
    // Fetch the variable location and set the parameter to 1.
    // Note that 1 is the number of the texture unit GL_TEXTURE1.
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
        glm::mat4 rotated_view = cs557::GetCamera().getViewMatrix();

        //----------------------------------------------------------------------------------------------------------------------------
        // This draws a coordinate system
        coordinateSystem.draw(projectionMatrix, rotated_view, modelMatrixCoord);


        // Activate the units and bind the textures per unit. 
        // The textures get processed by specific texture units. 
        // This code activates a texture unit and binds a texture to this unit. Thus, the texture gets processed. 
        // We can bind (process) multiple textures at the same time, but only one texture per texture unit. 
        // Removing these lines will stop the textures from being processed. 
        // Note that these lines are called every frame, which is not necessary as long as the texture content remains 
        // with the texture units. You could move the lines above the while loop and the texture process will still work in this case. 
        // OpenGL implements a state machine architecture, thus, the textures remain active as long as the state remains active.
       
        // The if-statement allow one to enable or disable the units via keyboard. 
        glActiveTexture(GL_TEXTURE1);
        if(tex1==1) glBindTexture(GL_TEXTURE_2D, texture_id1);
        else glBindTexture(GL_TEXTURE_2D, 0); // We bind no texture to this unit. The texture will disappear. 

        glActiveTexture(GL_TEXTURE0);
        if(tex0==1) glBindTexture(GL_TEXTURE_2D, texture_id0);
        else glBindTexture(GL_TEXTURE_2D, 0); // We bind no texture to this unit. The texture will disappear. 

        // draw the plane
        // If the textures are active and getting processed in the related fragment shader program, 
        // the textures will appear. 
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
