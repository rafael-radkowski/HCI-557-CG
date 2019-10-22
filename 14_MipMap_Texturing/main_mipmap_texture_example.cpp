/*

MipMap-Texture example.

This example demonstrates the implementation of mipmap textures with OpenGL and GLSL. 
The code requires OpenGL > 4.0

The scene renders a simple plane with a brick texturs attached to it. 
The OpenGL API glGenerateMipmap is used to automatically generate a mipmap texture for the loaded texture. 

The application is interactive. Use the keyboard 1-6 to switch the mipmap level automatically. 
Use the key 'c' to switch to a false-color rendering. 

Note the the mipmap process is implemented in mipmap_texture.fs and mipmap_texture.vs

This example includes multiple files:

- Texture2D.h/.cpp: the entire texturing process happens in this file. Study the code to understand it. 

- ModelPlane.h/.cpp: defines and creates the plane, including the vertices, normal vectors, and texture coordinates. 
 
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

// This is a helper variable. It points to a mipmap level override variable.
// inside the fragment shader. It allows us to manually set the texture mipmap level. 
int mipmap_level_override_location = -1;
int mipmap_level_override = 0;

int mipmap_false_color_location = -1;
int mipmap_false_color = 0;

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
   //cout << key << endl; ;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    else if(key == 48 && action == GLFW_PRESS) // 
    {
       mipmap_level_override = 0;
    }
    else if(key == 49 && action == GLFW_PRESS) // 1
    {
       mipmap_level_override = 1;
    }
    else if(key == 50 && action == GLFW_PRESS) // 2
    {
       mipmap_level_override = 2;
    }
    else if(key == 51 && action == GLFW_PRESS)  // 3
    {
       mipmap_level_override = 3;
    }
    else if(key == 52 && action == GLFW_PRESS) // 4
    {
        mipmap_level_override = 4;
    }
    else if(key == 53 && action == GLFW_PRESS)  // 5
    {
        mipmap_level_override = 5;
    }
    else if(key == 54 && action == GLFW_PRESS)  //6
    {
        mipmap_level_override = 6;
    }
    else if(key == 55 && action == GLFW_PRESS) // 7
    {

    }
    else if(key == 55 && action == GLFW_PRESS) // 8
    {

    }
    else if(key == 67 && action == GLFW_PRESS) // c
    {
        mipmap_false_color = ++mipmap_false_color%2;
    }

    glUseProgram(texture_program);
    glUniform1i(mipmap_level_override_location, mipmap_level_override);
    glUniform1i(mipmap_false_color_location, mipmap_false_color);
    glUseProgram(0);
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
    texture_program = cs557::LoadAndCreateShaderProgram("../mipmap_texture.vs", "../mipmap_texture.fs");


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
    

    // Load and create the  texture.  
    // The function is implemented in Texture2D.h/.cpp. Study the code. 
    // The function loads a texture from a file
    // The second variable points to the location to store the texture id. 
    // The third variable defines the clamp mode, and the last variable the texture unit. 
    // Note that texture unit 0 is used in this case. 
    LoadAndCreateTexture2D("../textures/texture_brick.bmp", &texture_id0, GL_CLAMP_TO_BORDER,  GL_TEXTURE0);

    // For MipMap, we need to change the min and mag filter. 
    // Set the MAG filter to linear, and the MIN filter to nearest neighbors,
    // GL_NEAREST_MIPMAP_NEAREST in particular. 
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST );

    // Generate mipmaps for the currently active texture. 
    glGenerateMipmap(GL_TEXTURE_2D);

    // The shader program uses the variable 'tex' to address the texture content. 
    // Fetch the variable location and set the parameter to 0.
    // Note that 0 is the number of the texture unit GL_TEXTURE0.
    int texture_location = glGetUniformLocation(texture_program, "tex");
    glUniform1i(texture_location, 0);


    // This is a helper function to manually set the mipmap level .
    mipmap_level_override_location = glGetUniformLocation(texture_program, "mipmap_level_override");
    glUniform1i(mipmap_level_override_location, 0);
    
    mipmap_false_color_location = glGetUniformLocation(texture_program, "mipmap_false_color");
    glUniform1i(mipmap_false_color_location, 0);
    

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
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_id0);

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
    cout << "This code example demonstrates MipMap texture mapping." << endl;
    cout << "Rafael Radkowski\nrafael@iastate.edu\nIowa State University" << endl;
    cout << "This example is part of ME/CS/CPRE557\n" << endl;

    cout << "Keyboard layout, use\n" << endl;
    cout << " 1-6: to manually switch between mipmap levels.\n" << endl;
    cout << " 0: to restore automatic mipmap level selection.\n" << endl;
    cout << " c: to enable/disable a false color rendering showing the mipmap levels used.\n" << endl;

    Init();

    Draw();

    return 1;
}
